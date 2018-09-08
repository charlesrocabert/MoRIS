/**
 * \file      Graph.cpp
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Graph class definition
 */

/************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ************************************************************************/

#include "Graph.h"


/*----------------------------
 * CONSTRUCTORS
 *----------------------------*/

/**
 * \brief    Constructor
 * \details  --
 * \param    Prng* prng
 * \param    type_of_data data
 * \param    std::string network_filename
 * \param    std::string map_filename
 * \param    std::string sample_filename
 * \param    int nb_repetitions
 * \param    std::vector<double>* road_linear_combination
 * \param    double minimal_connectivity
 * \return   \e void
 */
Graph::Graph( Prng* prng, type_of_data data, std::string network_filename, std::string map_filename, std::string sample_filename, int nb_repetitions, std::vector<double>* road_linear_combination, double minimal_connectivity )
{
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Set main parameters             */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _prng           = prng;
  _nb_repetitions = nb_repetitions;
  _data           = data;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Load the nodes                  */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _map.clear();
  _min_x_coord = 1e+10;
  _min_y_coord = 1e+10;
  _max_x_coord = 0.0;
  _max_y_coord = 0.0;
  std::ifstream file(map_filename.c_str(), std::ios::in);
  assert(file);
  std::string line;
  int    identifier = 0;
  double x_coord    = 0.0;
  double y_coord    = 0.0;
  double node_area  = 0.0;
  double suitable   = 0.0;
  int    intro      = 0;
  while(getline(file, line))
  {
    std::stringstream flux;
    flux.str(line.c_str());
    flux >> identifier >> x_coord >> y_coord >> node_area >> suitable >> intro;
    if (_min_x_coord > x_coord)
    {
      _min_x_coord = x_coord;
    }
    if (_min_y_coord > y_coord)
    {
      _min_y_coord = y_coord;
    }
    if (_max_x_coord < x_coord)
    {
      _max_x_coord = x_coord;
    }
    if (_max_y_coord < y_coord)
    {
      _max_y_coord = y_coord;
    }
    assert(_map.find(identifier) == _map.end());
    _map[identifier] = new Node(_prng, identifier, _nb_repetitions);
    _map[identifier]->set_map_data(x_coord, y_coord, node_area);
  }
  file.close();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Load the network                */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  file.open(network_filename.c_str(), std::ios::in);
  assert(file);
  std::string param_name;
  while(getline(file, line))
  {
    std::stringstream flux;
    flux.str(line.c_str());
    int    identifier1 = 0;
    int    identifier2 = 0;
    double roads1      = 0.0;
    double roads2      = 0.0;
    double roads3      = 0.0;
    double roads4      = 0.0;
    double roads5      = 0.0;
    double roads6      = 0.0;
    flux >> identifier1 >> identifier2 >> roads1 >> roads2 >> roads3 >> roads4 >> roads5 >> roads6;
    assert(roads1 >= 0.0);
    assert(roads2 >= 0.0);
    assert(roads3 >= 0.0);
    assert(roads4 >= 0.0);
    assert(roads5 >= 0.0);
    assert(roads6 >= 0.0);
    double weight = road_linear_combination->at(0)*roads1 + road_linear_combination->at(1)*roads2 + road_linear_combination->at(2)*roads3 + road_linear_combination->at(3)*roads4 + road_linear_combination->at(4)*roads5 + road_linear_combination->at(5)*roads6;
    if (weight < minimal_connectivity)
    {
      weight = minimal_connectivity;
    }
    if (identifier1 != -1 && identifier2 != -1)
    {
      assert(_map.find(identifier1) != _map.end());
      assert(_map.find(identifier2) != _map.end());
      _map[identifier1]->add_neighbor(weight, _map[identifier2]);
      _map[identifier2]->add_neighbor(weight, _map[identifier1]);
    }
    else if (identifier1 != -1 && identifier2 == -1)
    {
      assert(_map.find(identifier1) != _map.end());
      _map[identifier1]->add_neighbor(weight, NULL);
    }
    else if (identifier1 == -1 && identifier2 != -1)
    {
      assert(_map.find(identifier2) != _map.end());
      _map[identifier2]->add_neighbor(weight, NULL);
    }
  }
  file.close();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 4) Load the sample                 */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  file.open(sample_filename.c_str(), std::ios::in);
  assert(file);
  double y = 0.0;
  double n = 0.0;
  while(getline(file, line))
  {
    std::stringstream flux;
    flux.str(line.c_str());
    flux >> identifier >> y >> n;
    assert(_map.find(identifier) != _map.end());
    _map[identifier]->set_sample_data(y, n);
  }
  file.close();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 5) Compute the maximum weights sum */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _maximum_weights_sum = 0.0;
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    if (_maximum_weights_sum < it->second->get_weights_sum())
    {
      _maximum_weights_sum = it->second->get_weights_sum();
    }
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 6) Initialize other attributes     */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _minimization_score = 0.0;
  _it                 = _map.begin();
}

/*----------------------------
 * DESTRUCTORS
 *----------------------------*/

/**
 * \brief    Destructor
 * \details  --
 * \param    void
 * \return   \e void
 */
Graph::~Graph( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    delete it->second;
    it->second = NULL;
  }
  _map.clear();
}

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Update all the nodes
 * \details  --
 * \param    bool last_iteration
 * \return   \e void
 */
void Graph::update( bool last_iteration )
{
  /*~~~~~~~~~~~~~~~~~~~*/
  /* 1) Update nodes   */
  /*~~~~~~~~~~~~~~~~~~~*/
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->update();
  }
  
  /*~~~~~~~~~~~~~~~~~~~*/
  /* 2) Compute scores */
  /*~~~~~~~~~~~~~~~~~~~*/
  _minimization_score = 0.0;
  /* 2.1) If data is presence-only, compute the square-sum score */
  if (last_iteration && _data == PRESENCE_ONLY)
  {
    for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
    {
      double y = it->second->get_y();
      if (y > 0.0)
      {
        double nb_intros     = it->second->compute_mean_nb_introductions();
        _minimization_score += (y-nb_intros)*(y-nb_intros);
      }
    }
    _minimization_score = log10(_minimization_score);
  }
  /* 2.2) Else if data is presence-absence, compute the hypergeometric score */
  else if (last_iteration && _data == PRESENCE_ABSENCE)
  {
    for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
    {
      /*
      double y = it->second->get_y();
      double n = it->second->get_n();
      if (n > 0.0)
      {
        unsigned int a       = (unsigned int)(it->second->get_probability_of_presence()*_nb_repetitions);
        unsigned int b       = y;
        unsigned int c       = (unsigned int)_nb_repetitions-a;
        unsigned int d       = n-y;
        double current_FS    = gsl_ran_hypergeometric_pdf(a, a+b, c+d, a+c);
        double current_MFS   = gsl_ran_hypergeometric_pdf(b, b+b, d+d, b+d);
        _minimization_score += (1.0-current_FS/current_MFS)*(1.0-current_FS/current_MFS);
      }
       */
      double n    = it->second->get_n();
      double fobs = it->second->get_f();
      double fsim = it->second->get_probability_of_presence();
      if (n > 0.0)
      {
        _minimization_score += (fobs-fsim)*(fobs-fsim);
      }
    }
  }
  _minimization_score = log10(_minimization_score);
}

/**
 * \brief    Write node set state
 * \details  --
 * \param    int iteration
 * \param    double xintro
 * \param    double yintro
 * \return   \e void
 */
void Graph::write_state( int iteration, double xintro, double yintro )
{
  std::stringstream filename;
  filename << "state_" << iteration << ".txt";
  std::ofstream file(filename.str().c_str(), std::ios::out | std::ios::trunc);
  file << "id xcoord ycoord y n f_th f_obs xintro yintro\n";
  Node* node = get_first();
  while (node != NULL)
  {
    file << node->get_identifier() << " ";
    file << node->get_x_coord() << " ";
    file << node->get_y_coord() << " ";
    file << node->get_y() << " ";
    file << node->get_n() << " ";
    file << node->get_f() << " ";
    file << node->get_probability_of_presence() << " ";
    file << xintro << " ";
    file << yintro << "\n";
    node = get_next();
  }
  file.close();
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

