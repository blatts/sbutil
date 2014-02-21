// -*- mode: C++ -*-
// Time-stamp: "2012-10-22 09:11:50 sb"

/*
  file       HDF5File.hh
  copyright  (c) Sebastian Blatt 2012

*/


#ifndef HDF5FILE_HH__F5D661C6_1D11_4C45_9F50_95B86C097EE1
#define HDF5FILE_HH__F5D661C6_1D11_4C45_9F50_95B86C097EE1

#include <string>
#include <vector>

extern "C" {
#include <hdf5.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_complex.h>
}

namespace HDF5
{
  class Properties {
    private:
      const int identifier;
    protected:
      hid_t prop_id;
    public:
      Properties(const int identifier_);
      virtual ~Properties();
      hid_t GetId() const {return prop_id;}
  };

  class PropLinkCreate : public Properties {
    public:
      PropLinkCreate();
      virtual ~PropLinkCreate();
      void SetIntermediateCreate(bool create_intermediate_groups);
  };

  class PropDatasetCreate : public Properties {
    public:
      PropDatasetCreate();
      virtual ~PropDatasetCreate();
      void SetChunk(const std::vector<size_t>& dims,
                    size_t max_chunk_size);
      void SetDeflate(int level);
  };

  class Dataspace {
    protected:
      hid_t dataspace_id;
    public:
      Dataspace(const std::vector<size_t>& dims);
      virtual ~Dataspace();
      hid_t GetId() const {return dataspace_id;}
  };

  class File;
  class Group {
    protected:
      hid_t group_id;
    public:
      Group();
      virtual ~Group();
      hid_t GetId() const {return group_id;}
      void Create(File& file,
                  const std::string& name,
                  PropLinkCreate& prop_link_create);
      void Create(Group& group,
                  const std::string& name,
                  PropLinkCreate& prop_link_create);
      bool Open(File& file,
                const std::string& name);
      bool Open(Group& group,
                const std::string& name);
    private:
      void Create(hid_t parent,
                  const std::string& name,
                  PropLinkCreate& prop_link_create);
      bool Open(hid_t parent,
                const std::string& name);
  };

  class Dataset {
    protected:
      hid_t dataset_id;
    public:
      Dataset();
      virtual ~Dataset();

      void Create(Group& group,
                  const std::string& name,
                  PropDatasetCreate& prop_dataset_create,
                  Dataspace& dataspace);
      void Create(File& file,
                  const std::string& name,
                  PropDatasetCreate& prop_dataset_create,
                  Dataspace& dataspace);
      void Write(const double* data);
      void Write(const int* data);
    private:
      void Create(hid_t parent,
                  const std::string& name,
                  PropDatasetCreate& prop_dataset_create,
                  Dataspace& dataspace);
  };


  class File {
    private:
      std::string filename;
      hid_t file_id;
      size_t max_chunk_size;

    public:
      File(const std::string& filename_, bool append_);
      ~File();
      void Open(const std::string& filename_, bool append_);
      void Close();

      void WriteDatasetDouble(const std::string& path,
                              const std::string& name,
                              const std::vector<size_t>& dimensions,
                              const double* data);
      void WriteDatasetDouble(const std::string& path,
                              const std::string& name,
                              const std::vector<size_t>& dimensions,
                              const std::vector<double>& data){
        WriteDatasetDouble(path, name, dimensions, &data[0]);
      }

      void WriteDatasetInt(const std::string& path,
                           const std::string& name,
                           const std::vector<size_t>& dimensions,
                           const int* data);
      void WriteDatasetInt(const std::string& path,
                           const std::string& name,
                           const std::vector<size_t> dimensions,
                           const std::vector<int>& data){
        WriteDatasetInt(path, name, dimensions, &data[0]);
      }

      void WriteVectorDouble(const std::string& path,
                             const std::string& name,
                             const std::vector<double>& data);
      void WriteVectorInt(const std::string& path,
                          const std::string& name,
                          const std::vector<int>& data);

      void WriteGSLVector(const std::string& path,
                          const std::string& name,
                          const gsl_vector* v);
      void WriteGSLVector(const std::string& path,
                          const std::string& name,
                          const gsl_vector_complex* v);

      void WriteGSLMatrix(const std::string& path,
                          const std::string& name,
                          const gsl_matrix* matrix);
      void WriteGSLMatrix(const std::string& path,
                          const std::string& name,
                          const gsl_matrix_complex* matrix);

      hid_t GetId() const {return file_id;}
  };

}


#endif // HDF5FILE_HH__F5D661C6_1D11_4C45_9F50_95B86C097EE1

// HDF5File.hh ends here
