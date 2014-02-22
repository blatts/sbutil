// -*- mode: C++ -*-
// Time-stamp: "2014-02-21 19:29:55 sb"

/*
  file       HDF5File.cc
  copyright  (c) Sebastian Blatt 2012, 2013, 2014

 */


#define COMPILING_THIS_SHIT = 1
#include "HDF5File.hh"
#undef COMPILING_THIS_SHIT

#include "Exception.hh"
#include "OutputManipulator.hh"
#include "WrappedCall.hh"


#include <sstream>
#include <cassert>

using namespace HDF5;

// Convenience macro to check HDF5 API calls that return herr_t < 0 on
// failure (all of them)
#define H5CALL(f, ...) WCALL(f, ValidatorNonnegative<herr_t>, __VA_ARGS__)

// turn off HDF5 error messages while alive. Turn back on when
// destroyed.
class ErrorMessageSuppressor{
  private:
    H5E_auto1_t func;
    void* client_data;
  public:
    ErrorMessageSuppressor();
    ~ErrorMessageSuppressor();
};

ErrorMessageSuppressor::ErrorMessageSuppressor(){
  // Buffer previous state
  H5CALL(H5Eget_auto1, &func, &client_data);
  H5CALL(H5Eset_auto1, NULL, NULL);
}

ErrorMessageSuppressor::~ErrorMessageSuppressor(){
  // Turn error messages back on
  H5CALL(H5Eset_auto1, func, client_data);
}

// ------------------------------------------------------------------ Properties

Properties::Properties(const int identifier_)
  : identifier(identifier_),
    prop_id(0)
{
  if((prop_id = H5Pcreate(identifier)) < 0){
    std::ostringstream os;
    os << "H5Pcreate(" << identifier << ") failed.";
    throw EXCEPTION(os.str());
  }
}

Properties::~Properties() {
  if(prop_id){
    H5Pclose(prop_id);
    prop_id = 0;
  }
}

// -------------------------------------------------------------- PropLinkCreate

PropLinkCreate::PropLinkCreate()
  : Properties(H5P_LINK_CREATE)
{
  SetIntermediateCreate(true);
}

PropLinkCreate::~PropLinkCreate(){}


void PropLinkCreate::SetIntermediateCreate(bool create_intermediate_groups) {
  H5CALL(H5Pset_create_intermediate_group, prop_id,
         create_intermediate_groups ? 1 : 0);
}

// ------------------------------------------------------------ PropDatasetCreate

PropDatasetCreate::PropDatasetCreate()
  : Properties(H5P_DATASET_CREATE)
{}

PropDatasetCreate::~PropDatasetCreate() {}

void PropDatasetCreate::SetChunk(const std::vector<size_t>& dims,
                                 size_t max_chunk_size)
{
  std::vector<hsize_t> dc(dims.size());
  for(size_t i=0; i<dims.size(); ++i){
    if(i < 2){
      dc[i] = std::min(dims[i], max_chunk_size);
    }
    // FIXME: this should be handled better.
    else{
      dc[i] = 1;
    }
  }
  H5CALL(H5Pset_chunk, prop_id, dc.size(), &dc[0]);
}

void PropDatasetCreate::SetDeflate(int level){
  H5CALL(H5Pset_deflate, prop_id, level);
}

// -------------------------------------------------------------------- Dataspace

Dataspace::Dataspace(const std::vector<size_t>& dims)
  : dataspace_id(0)
{
  std::vector<hsize_t> d(dims.size());
  for(size_t i=0; i<d.size(); ++i){
    d[i] = dims[i];
  }
  if((dataspace_id = H5Screate_simple(d.size(), &d[0], NULL)) < 0){
    std::ostringstream os;
    os << "H5Screate_simple(" << vector_form<hsize_t>(d) << ") failed.";
    throw EXCEPTION(os.str());
  }
}

Dataspace::~Dataspace(){
  if(dataspace_id){
    H5Sclose(dataspace_id);
    dataspace_id = 0;
  }
}

// ------------------------------------------------------------------------ Group

Group::Group()
  : group_id(0)
{}

Group::~Group(){
  if(group_id){
    H5Gclose(group_id);
    group_id = 0;
  }
}

void Group::Create(const hid_t parent,
                   const std::string& name,
                   PropLinkCreate& prop_link_create)
{
  if((group_id = H5Gcreate(parent,
                           name.c_str(),
                           prop_link_create.GetId(),
                           H5P_DEFAULT,
                           H5P_DEFAULT)) < 0)
  {
    std::ostringstream os;
    os << "H5Gcreate(" << name << ") failed.";
    throw EXCEPTION(os.str());
  }
}

void Group::Create(File& file,
                   const std::string& name,
                   PropLinkCreate& prop_link_create)
{
  Create(file.GetId(), name, prop_link_create);
}

void Group::Create(Group& group,
                   const std::string& name,
                   PropLinkCreate& prop_link_create)
{
  Create(group.GetId(), name, prop_link_create);
}

bool Group::Open(const hid_t parent, const std::string& name){
  ErrorMessageSuppressor e;
  if((group_id = H5Gopen(parent, name.c_str(), H5P_DEFAULT)) < 0){
    group_id = 0;
    return false;
  }
  return true;
}

bool Group::Open(Group& group, const std::string& name){
  return Open(group.GetId(), name);
}

bool Group::Open(File& file, const std::string& name){
  return Open(file.GetId(), name);
}



// ---------------------------------------------------------------------- Dataset


Dataset::Dataset()
  : dataset_id(0)
{}

Dataset::~Dataset() {
  if(dataset_id){
    H5Dclose(dataset_id);
    dataset_id = 0;
  }
}

void Dataset::Create(hid_t parent,
                     const std::string& name,
                     PropDatasetCreate& prop_dataset_create,
                     Dataspace& dataspace)
{
  if((dataset_id = H5Dcreate(parent,
                             name.c_str(),
                             H5T_IEEE_F64LE,
                             dataspace.GetId(),
                             H5P_DEFAULT,
                             prop_dataset_create.GetId(),
                             H5P_DEFAULT)) < 0)
  {
    std::ostringstream os;
    os << "H5Dcreate(" << name << ") failed.";
    throw EXCEPTION(os.str());
  }
}

void Dataset::Create(Group& group,
                     const std::string& name,
                     PropDatasetCreate& prop_dataset_create,
                     Dataspace& dataspace)
{
  Create(group.GetId(), name, prop_dataset_create, dataspace);
}

void Dataset::Create(File& file,
                     const std::string& name,
                     PropDatasetCreate& prop_dataset_create,
                     Dataspace& dataspace)
{
  Create(file.GetId(), name, prop_dataset_create, dataspace);
}

void Dataset::Write(const double* data){
  H5CALL(H5Dwrite, dataset_id, H5T_NATIVE_DOUBLE,
         H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
}

void Dataset::Write(const int* data){
  H5CALL(H5Dwrite, dataset_id, H5T_NATIVE_INT,
         H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
}


// ------------------------------------------------------------------------- File


File::File(const std::string& filename_,
           bool append_)
  : filename(filename_),
    file_id(0),
    max_chunk_size(64)
{
  Open(filename_, append_);
}

File::~File(){
  Close();
}

void File::Open(const std::string& filename_, bool append_){
  hid_t id = 0;
  if(append_){
    { // try opening for read-write access first. Fail quietly and then try
      // creating the file.
      ErrorMessageSuppressor e;
      id = H5Fopen(filename_.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
    }
    if(id < 0){
      if((id = H5Fcreate(filename_.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0){
        std::ostringstream os;
        os << "H5Fcreate(" << filename_ << ", H5F_ACC_TRUNC, H5P_DEFAULT, "
           << "H5P_DEFAULT) failed.";
        throw EXCEPTION(os.str());
      }
    }
  }
  else{
    if((id = H5Fcreate(filename_.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT)) < 0){
      std::ostringstream os;
      os << "H5Fcreate(" << filename_ << ", H5F_ACC_TRUNC, H5P_DEFAULT, "
         << "H5P_DEFAULT) failed.";
      throw EXCEPTION(os.str());
    }
  }
  filename = filename_;
  file_id = id;
}

void File::Close(){
  if(file_id){
    H5Fclose(file_id);
    file_id = 0;
  }
}

static bool path_is_root(const std::string& path){
  return path == "" || path == "/";
}

void File::WriteDatasetDouble(const std::string& path,
                              const std::string& name,
                              const std::vector<size_t>& dimensions,
                              const double* data)
{
  PropLinkCreate p_link_create;

  Group group;
  if(!path_is_root(path)){
    if(!group.Open(*this, path)){
      group.Create(*this, path, p_link_create);
    }
  }

  Dataspace dataspace(dimensions);

  PropDatasetCreate p_dataset_create;
  p_dataset_create.SetChunk(dimensions, max_chunk_size);
  p_dataset_create.SetDeflate(9);

  Dataset dataset;
  if(!path_is_root(path)){
    dataset.Create(group, name, p_dataset_create, dataspace);
  }
  else{
    dataset.Create(*this, name, p_dataset_create, dataspace);
  }
  dataset.Write(data);
}

void File::WriteDatasetInt(const std::string& path,
                           const std::string& name,
                           const std::vector<size_t>& dimensions,
                           const int* data)
{
  PropLinkCreate p_link_create;

  Group group;
  if(!path_is_root(path)){
    if(!group.Open(*this, path)){
      group.Create(*this, path, p_link_create);
    }
  }

  Dataspace dataspace(dimensions);

  PropDatasetCreate p_dataset_create;
  p_dataset_create.SetChunk(dimensions, max_chunk_size);
  p_dataset_create.SetDeflate(9);

  Dataset dataset;
  if(!path_is_root(path)){
    dataset.Create(group, name, p_dataset_create, dataspace);
  }
  else{
    dataset.Create(*this, name, p_dataset_create, dataspace);
  }
  dataset.Write(data);
}



void File::WriteVectorDouble(const std::string& path,
                             const std::string& name,
                             const std::vector<double>& data){
  std::vector<size_t> dim(1);
  dim[0] = data.size();
  WriteDatasetDouble(path, name, dim, data);
}

void File::WriteVectorInt(const std::string& path,
                          const std::string& name,
                          const std::vector<int>& data){
  std::vector<size_t> dim(1);
  dim[0] = data.size();
  WriteDatasetInt(path, name, dim, data);
}


void File::WriteGSLVector(const std::string& path,
                          const std::string& name,
                          const gsl_vector* v)
{
  assert(v);
  std::vector<size_t> dim(1, 0u);
  dim[0] = v->size;
  WriteDatasetDouble(path, name, dim, v->data);
}

void File::WriteGSLVector(const std::string& path,
                          const std::string& name,
                          const gsl_vector_complex* v)
{
  assert(v);
  std::vector<size_t> dim(2, 0u);
  dim[0] = v->size;
  dim[1] = 2;
  WriteDatasetDouble(path, name, dim, v->data);
}



void File::WriteGSLMatrix(const std::string& path,
                          const std::string& name,
                          const gsl_matrix* matrix)
{
  assert(matrix);
  std::vector<size_t> dims(2, 0u);
  dims[0] = matrix->size1;
  dims[1] = matrix->size2;
  WriteDatasetDouble(path, name, dims, matrix->data);
}

void  File::WriteGSLMatrix(const std::string& path,
                           const std::string& name,
                           const gsl_matrix_complex* matrix)
{
  assert(matrix);
  std::vector<size_t> dims(3, 0u);
  dims[0] = matrix->size1;
  dims[1] = matrix->size2;
  dims[2] = 2;
  WriteDatasetDouble(path, name, dims, matrix->data);
}

// HDF5File.cc ends here
