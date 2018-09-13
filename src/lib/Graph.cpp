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
 * \param    Parameters* parameters
 * \return   \e void
 */
Graph::Graph( Parameters* parameters )
{
  /*--------------------------------------- MAIN PARAMETERS */
  
  assert(parameters != NULL);
  _parameters = parameters;
  
  /*--------------------------------------- GRAPH STRUCTURE */
  
  load_map();
  load_network();
  load_sample();
  
  /*--------------------------------------- GRAPH STATISTICS */
  
  _introduction_node = get_introduction_node_from_coordinates();
  compute_maximum_weight_sum();
  compute_jump_probability();
  reset_states();
  set_introduction_node();
  
  /*--------------------------------------- MINIMIZATION SCORE */
  
  compute_score();
  _empty_score = _score;
  _score       = 0.0;
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
 * \brief    Untag all the nodes
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::untag( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->untag();
  }
}

/**
 * \brief    Update all the node states
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::update_state( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->update_state();
  }
}

/**
 * \brief    Compute the score
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::compute_score( void )
{
  _score = 0.0;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) If data is presence-only, compute the LEAST SQUARE SUM score       */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  if (_parameters->get_type_of_data() == PRESENCE_ONLY)
  {
    for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
    {
      double y_obs = it->second->get_y_obs();
      if (y_obs > 0.0)
      {
        it->second->compute_mean_var_nb_introductions();
        double nb_intros  = it->second->get_mean_nb_introductions();
        _score           += (y_obs-nb_intros)*(y_obs-nb_intros);
      }
    }
  }
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Else if data is presence-absence, compute the HYPERGEOMETRIC score */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  else if (_parameters->get_type_of_data() == PRESENCE_ABSENCE)
  {
    double count = 0.0;
    for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
    {
      double y_sim = it->second->get_y_sim();
      double n_sim = it->second->get_n_sim();
      double y_obs = it->second->get_y_obs();
      double n_obs = it->second->get_n_obs();
      if (n_obs > 0.0)
      {
        unsigned int a      = (unsigned int)(y_sim);
        unsigned int b      = (unsigned int)(y_obs);
        unsigned int c      = (unsigned int)(n_sim-y_sim);
        unsigned int d      = (unsigned int)(n_obs-y_obs);
        double current_FS   = gsl_ran_hypergeometric_pdf(a, a+b, c+d, a+c);
        double current_MFS  = gsl_ran_hypergeometric_pdf(b, b+b, d+d, b+d);
        _score             += (1.0-current_FS/current_MFS)*(1.0-current_FS/current_MFS);
        count              += 1.0;
      }
    }
    _score /= count;
    _score = log10(_score);
  }
}

/**
 * \brief    Write node set state
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
void Graph::write_state( std::string filename )
{
  std::ofstream file(filename, std::ios::out | std::ios::trunc);
  file << "id xcoord ycoord y n f_obs n_sim y_sim f_sim total_nb_intros mean_nb_intros var_nb_intros empty_score score\n";
  Node* node = get_first();
  while (node != NULL)
  {
    file << node->get_identifier() << " ";
    file << node->get_x_coord() << " ";
    file << node->get_y_coord() << " ";
    file << node->get_y_obs() << " ";
    file << node->get_n_obs() << " ";
    file << node->get_f_obs() << " ";
    file << node->get_n_sim() << " ";
    file << node->get_y_sim() << " ";
    file << node->get_f_sim() << " ";
    file << node->get_total_nb_introductions() << " ";
    file << node->get_mean_nb_introductions() << " ";
    file << node->get_var_nb_introductions() << " ";
    file << _empty_score << " ";
    file << _score << "\n";
    node = get_next();
  }
  file.close();
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

/**
 * \brief    Get the introduction node from the coordinates
 * \details  --
 * \param    void
 * \return   \e void
 */
int Graph::get_introduction_node_from_coordinates( void )
{
  std::pair<double, double>* coordinates = _parameters->get_introduction_coordinates();
  double                     min_dist    = 1e+10;
  int                        min_intro   = 0;
  Node*                      node        = get_first();
  while (node != NULL)
  {
    double dist = sqrt((coordinates->first-node->get_x_coord())*(coordinates->first-node->get_x_coord()) + (coordinates->second-node->get_y_coord())*(coordinates->second-node->get_y_coord()));
    if (min_dist > dist)
    {
      min_dist  = dist;
      min_intro = node->get_identifier();
    }
    node = get_next();
  }
  return min_intro;
}

/**
 * \brief    Load the map from file
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::load_map( void )
{
  _map.clear();
  _min_x_coord = 1e+10;
  _max_x_coord = 0.0;
  _min_y_coord = 1e+10;
  _max_y_coord = 0.0;
  std::ifstream file(_parameters->get_map_filename(), std::ios::in);
  assert(file);
  std::string line;
  int    identifier    = 0;
  double x_coord       = 0.0;
  double y_coord       = 0.0;
  double node_area     = 0.0;
  double suitable_area = 0.0;
  while(getline(file, line))
  {
    std::stringstream flux;
    flux.str(line.c_str());
    flux >> identifier >> x_coord >> y_coord >> node_area >> suitable_area;
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
    _map[identifier] = new Node(_parameters->get_prng(), identifier, _parameters->get_repetitions_by_simulation(), _parameters->get_introduction_probability());
    _map[identifier]->set_map_data(x_coord, y_coord, node_area, suitable_area);
  }
  file.close();
}

/**
 * \brief    Load the network from file
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::load_network( void )
{
  std::ifstream file(_parameters->get_network_filename(), std::ios::in);
  assert(file);
  std::string line;
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
    double weight  = 0.0;
    weight        += _parameters->get_road_linear_combination()->at(0)*roads1;
    weight        += _parameters->get_road_linear_combination()->at(1)*roads2;
    weight        += _parameters->get_road_linear_combination()->at(2)*roads3;
    weight        += _parameters->get_road_linear_combination()->at(3)*roads4;
    weight        += _parameters->get_road_linear_combination()->at(4)*roads5;
    weight        += _parameters->get_road_linear_combination()->at(5)*roads6;
    if (weight < _parameters->get_minimal_connectivity())
    {
      weight = _parameters->get_minimal_connectivity();
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
}

/**
 * \brief    Load the sample from file
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::load_sample( void )
{
  std::ifstream file(_parameters->get_sample_filename(), std::ios::in);
  assert(file);
  std::string line;
  int    identifier = 0;
  double y          = 0.0;
  double n          = 0.0;
  while(getline(file, line))
  {
    std::stringstream flux;
    flux.str(line.c_str());
    flux >> identifier >> y >> n;
    assert(_map.find(identifier) != _map.end());
    _map[identifier]->set_sample_data(y, n);
  }
  file.close();
}

/**
 * \brief    Compute the maximum weight sum
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::compute_maximum_weight_sum( void )
{
  _maximum_weights_sum = 0.0;
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    if (_maximum_weights_sum < it->second->get_weights_sum())
    {
      _maximum_weights_sum = it->second->get_weights_sum();
    }
  }
}

/**
 * \brief    Compute the jump probability
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::compute_jump_probability( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->set_jump_probability(it->second->get_weights_sum()/_maximum_weights_sum);
    //it->second->set_jump_probability(1.0);
  }
}

/**
 * \brief    Reset all node states
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::reset_states( void )
{
  for (std::unordered_map<int, Node*>::iterator it = _map.begin(); it != _map.end(); ++it)
  {
    it->second->reset_state();
  }
  _score = 0.0;
}

/**
 * \brief    Set the introduction node
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::set_introduction_node( void )
{
  _map[_introduction_node]->set_as_introduction_node();
}
