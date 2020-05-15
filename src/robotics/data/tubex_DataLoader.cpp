/** 
 *  DataLoader class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <algorithm>
#include "tubex_DataLoader.h"
#include "tubex_Exception.h"

#define DATA_FILE_EXTENSION ".tubex"

using namespace std;
using namespace ibex;

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

  vector<Beacon> DataLoader::generate_landmarks(const IntervalVector& map_box, int nb_landmarks)
  {
    assert(map_box.size() == 2);
    assert(nb_landmarks >= 0);

    srand(time(NULL));
    vector<Beacon> v_beacons;
    for(int i = 0 ; i < nb_landmarks ; i++)
    {
      double x = map_box[0].lb() + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * map_box[0].diam();
      double y = map_box[1].lb() + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * map_box[1].diam();
      v_beacons.push_back(Beacon(x, y));
    }

    return v_beacons;
  }

  vector<IntervalVector> DataLoader::generate_landmarks_boxes(const IntervalVector& map_box, int nb_landmarks)
  {
    assert(map_box.size() == 2);
    assert(nb_landmarks >= 0);

    vector<Beacon> v_map = DataLoader::generate_landmarks(map_box, nb_landmarks);
    vector<IntervalVector> v_boxes;
    for(const auto& b : v_map)
      v_boxes.push_back(b.pos_box().subvector(0,1)); // todo: set everything in 2d

    return v_boxes;
  }
  
  vector<IntervalVector> DataLoader::generate_observations(const Vector& x, const vector<IntervalVector>& map, bool random, const Interval& visi_range, const Interval& visi_angle)
  {
    assert(x.size() == 3);
    assert(map.size() > 0);

    vector<IntervalVector> v_obs;
    vector<IntervalVector> random_map(map);
    if(random)
      std::random_shuffle(random_map.begin(), random_map.end());

    for(size_t i = 0 ; i < random_map.size() ; i++)
    {
      Interval r = sqrt(ibex::pow(x[0]-random_map[i][0],2) + ibex::pow(x[1]-random_map[i][1],2));
      Interval a = ibex::atan2(random_map[i][1]-x[1], random_map[i][0]-x[0]) - x[2];
      
      if(visi_range.intersects(r) && visi_angle.intersects(a)) // if the beacon is seen by the robot
      {
        IntervalVector obs(2);
        obs[0] = r; obs[1] = a;
        v_obs.push_back(obs);
      }
    }
    
    return v_obs;
  }
  
  vector<IntervalVector> DataLoader::generate_observations(const Vector& x, const vector<Beacon>& map, bool random, const Interval& visi_range, const Interval& visi_angle)
  {
    vector<IntervalVector> map_boxes;
    for(auto& beacon : map)
      map_boxes.push_back(beacon.pos());
    return generate_observations(x, map_boxes, random, visi_range, visi_angle);
  }
  
  vector<IntervalVector> DataLoader::generate_observations(const TrajectoryVector& x, const vector<Beacon>& map, int nb_obs, bool random, const Interval& visi_range, const Interval& visi_angle, const Interval& tdomain)
  {
    assert(x.size() >= 2);
    assert(nb_obs >= 0);
    assert(map.size() > 0 || nb_obs == 0); // if no observation, no beacon needed

    float dt = 0.001;
    vector<IntervalVector> v_obs;

    if(nb_obs == 0)
      return v_obs;

    vector<Beacon> random_map(map);

    Interval tdomain_ = x.tdomain() & tdomain;
    for(double t = tdomain_.lb() ; t < tdomain_.ub()-dt ; t+= tdomain_.diam() / nb_obs)
    {
      if(random)
        std::random_shuffle(random_map.begin(), random_map.end());

      for(size_t i = 0 ; i < random_map.size() ; i++)
      {
        Interval t_(t);
        Interval r = sqrt(pow(x[0](t_) - random_map[i].pos()[0], 2) + pow(x[1](t_) - random_map[i].pos()[1], 2));
        Interval heading = atan2(x[1](t+dt) - x[1](t_), x[0](t+dt) - x[0](t_));
        Interval a = atan2(random_map[i].pos()[1] - x[1](t_), random_map[i].pos()[0] - x[0](t_)) - heading;
        
        if(visi_range.intersects(r) && visi_angle.intersects(a)) // if the beacon is seen by the robot
        {
          IntervalVector obs(3);
          obs[0] = t; obs[1] = r; obs[2] = a;
          v_obs.push_back(obs);
          break;
        }
      }
    }
    
    return v_obs;
  }
  
  vector<IntervalVector> DataLoader::generate_observations(const TrajectoryVector& x, const vector<IntervalVector>& map, int nb_obs, bool random, const Interval& visi_range, const Interval& visi_angle, const Interval& tdomain)
  {
    vector<Beacon> map_beacons;
    for(auto& b : map)
      map_beacons.push_back(Beacon(b));
    return generate_observations(x, map_beacons, nb_obs, random, visi_range, visi_angle, tdomain);
  }
}