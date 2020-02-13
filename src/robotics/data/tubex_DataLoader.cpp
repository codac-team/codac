/** 
 *  DataLoader class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "tubex_DataLoader.h"
#include "tubex_Exception.h"

#define DATA_FILE_EXTENSION ".tubex"

using namespace std;

namespace tubex
{
  DataLoader::DataLoader()
  {

  }

  DataLoader::DataLoader(const string& file_path)
    : m_file_path(file_path)
  {
    m_datafile = new ifstream();
    m_datafile->open(file_path.c_str(), std::ios_base::out);
    if(!m_datafile->is_open())
      throw Exception("DataLoader constructor", "unable to load data file");
  }

  DataLoader::~DataLoader()
  {
    if(m_datafile != NULL)
      delete m_datafile;
  }

  void DataLoader::serialize_data(const TubeVector& x, const TrajectoryVector& traj) const
  {
    x.serialize(m_file_path + DATA_FILE_EXTENSION, traj);
  }
  
  bool DataLoader::serialized_data_available() const
  {
    if(FILE *file = fopen((m_file_path + DATA_FILE_EXTENSION).c_str(), "r"))
    {
      fclose(file);
      return true;
    }

    else
      return false;
  }
  
  void DataLoader::deserialize_data(TubeVector *&x, TrajectoryVector *&traj) const
  {
    cout << " (deserialization)" << flush;
    x = new TubeVector(m_file_path + DATA_FILE_EXTENSION, traj);
  }
}