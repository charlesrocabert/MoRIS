/**
 * \file      Graph.cpp
 * \author    Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright © 2014-2022 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Graph class definition
 */

/****************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright © 2014-2022 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * Web: https://github.com/charlesrocabert/MoRIS
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
 ****************************************************************************/

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
  compute_statistics();
  compute_human_activity_index();
  reset_states();
  set_introduction_node();
  
  /*--------------------------------------- MINIMIZATION SCORE */
  
  _total_log_likelihood         = 0.0;
  _total_log_empty_likelihood   = 0.0;
  _total_log_maximum_likelihood = 0.0;
  _empty_score                  = 0.0;
  _score                        = 0.0;
  compute_score(true);
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
  Node* node = get_first();
  while (node != NULL)
  {
    node->untag();
    node = get_next();
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
  Node* node = get_first();
  while (node != NULL)
  {
    node->update_state();
    node = get_next();
  }
}

/**
 * \brief    Compute the score
 * \details  --
 * \param    bool empty
 * \return   \e void
 */
void Graph::compute_score( bool empty )
{
  _total_log_likelihood         = 0.0;
  _total_log_maximum_likelihood = 0.0;
  _score                        = 0.0;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) If data is presence-only, compute the LEAST SQUARE SUM score       */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  if (_parameters->get_typeofdata() == PRESENCE_ONLY)
  {
    Node* node = get_first();
    while (node != NULL)
    {
      double y_obs = node->get_y_obs();
      if (y_obs > 0.0)
      {
        double nb_intros  = node->get_mean_nb_introductions();
        _score           += (y_obs-nb_intros)*(y_obs-nb_intros);
      }
      node = get_next();
    }
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Else if data is presence-absence, compute the HYPERGEOMETRIC score */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  else if (_parameters->get_typeofdata() == PRESENCE_ABSENCE)
  {
    Node* node = get_first();
    while (node != NULL)
    {
      if (node->get_n_obs() > 0.0)
      {
        node->compute_score();
        _total_log_likelihood         += node->get_log_likelihood();
        _total_log_maximum_likelihood += node->get_log_maximum_likelihood();
        _score                        += node->get_score();
      }
      node = get_next();
    }
  }
  if (empty)
  {
    _total_log_empty_likelihood = _total_log_likelihood;
    _empty_score                = _score;
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
  file << "id x y y_obs n_obs p_obs total_nb_intros mean_nb_intros var_nb_intros y_sim n_sim p_sim mean_first_invasion var_first_invasion mean_last_invasion var_last_invasion L empty_L max_L empty_score score\n";
  Node* node = get_first();
  while (node != NULL)
  {
    file << node->get_identifier() << " ";
    file << node->get_x() << " ";
    file << node->get_y() << " ";
    file << node->get_y_obs() << " ";
    file << node->get_n_obs() << " ";
    file << node->get_p_obs() << " ";
    file << node->get_total_nb_introductions() << " ";
    file << node->get_mean_nb_introductions() << " ";
    file << node->get_var_nb_introductions() << " ";
    file << node->get_y_sim() << " ";
    file << node->get_n_sim() << " ";
    file << node->get_p_sim() << " ";
    file << node->get_mean_first_invasion_age() << " ";
    file << node->get_var_first_invasion_age() << " ";
    file << node->get_mean_last_invasion_age() << " ";
    file << node->get_var_last_invasion_age() << " ";
    file << _total_log_likelihood << " ";
    file << _total_log_empty_likelihood << " ";
    file << _total_log_maximum_likelihood << " ";
    file << _empty_score << " ";
    file << _score << "\n";
    node = get_next();
  }
  file.close();
}

/**
 * \brief    Write the euclidean distance distribution of invaded nodes (simulated or observed)
 * \details  --
 * \param    std::string observed_filename
 * \param    std::string simulated_filename
 * \return   \e void
 */
void Graph::write_invasion_euclidean_distributions( std::string observed_filename, std::string simulated_filename )
{
  /*~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Observed invasion  */
  /*~~~~~~~~~~~~~~~~~~~~~~~*/
  std::ofstream file(observed_filename, std::ios::out | std::ios::trunc);
  file << "start_node end_node euclidean_dist\n";
  for (std::unordered_map<int, Node*>::iterator it1 = _map.begin(); it1 != _map.end(); ++it1)
  {
    for (std::unordered_map<int, Node*>::iterator it2 = _map.find(it1->first); it2 != _map.end(); ++it2)
    {
      if (it1->first != it2->first)
      {
        double dist = compute_euclidean_distance(it1->second, it2->second);
        if (it1->second->get_y_obs() > 0.0 && it2->second->get_y_obs() > 0.0)
        {
          for (double i = 0; i < it1->second->get_y_obs(); i++)
          {
            file << it1->first << " " << it2->first << " " << dist << "\n";
          }
        }
      }
    }
  }
  file.close();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Simulated invasion */
  /*~~~~~~~~~~~~~~~~~~~~~~~*/
  file.open(simulated_filename, std::ios::out | std::ios::trunc);
  file << "start_node end_node euclidean_dist rep\n";
  for (std::unordered_map<int, Node*>::iterator it1 = _map.begin(); it1 != _map.end(); ++it1)
  {
    for (std::unordered_map<int, Node*>::iterator it2 = _map.find(it1->first); it2 != _map.end(); ++it2)
    {
      if (it1->first != it2->first)
      {
        double dist = compute_euclidean_distance(it1->second, it2->second);
        for (int i = 0; i < _parameters->get_repetitions(); i++)
        {
          if (it1->second->isOccupied(i) && it2->second->isOccupied(i))
          {
            file << it1->first << " " << it2->first << " " << dist << " " << i+1 << "\n";
          }
        }
      }
    }
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
  double x_intro   = _parameters->get_x_introduction();
  double y_intro   = _parameters->get_y_introduction();
  double min_dist  = 1e+10;
  int    min_intro = 0;
  Node*  node      = get_first();
  while (node != NULL)
  {
    double dist = sqrt((x_intro-node->get_x())*(x_intro-node->get_x()) + (y_intro-node->get_y())*(y_intro-node->get_y()));
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
  std::ifstream file(_parameters->get_map_filename(), std::ios::in);
  assert(file);
  std::string line;
  int    identifier         = 0;
  double x_coord            = 0.0;
  double y_coord            = 0.0;
  double node_area          = 0.0;
  double suitable_area      = 0.0;
  double population         = 0.0;
  double population_density = 0.0;
  double road_density       = 0.0;
  while(getline(file, line))
  {
    std::stringstream flux;
    flux.str(line.c_str());
    flux >> identifier >> x_coord >> y_coord >> node_area >> suitable_area >> population >> population_density >> road_density;
    assert(_map.find(identifier) == _map.end());
    _map[identifier] = new Node(_parameters, identifier);
    _map[identifier]->set_map_data(x_coord, y_coord, node_area, suitable_area, population, population_density, road_density);
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
    weight        += _parameters->get_w1()*roads1;
    weight        += _parameters->get_w2()*roads2;
    weight        += _parameters->get_w3()*roads3;
    weight        += _parameters->get_w4()*roads4;
    weight        += _parameters->get_w5()*roads5;
    weight        += _parameters->get_w6()*roads6;
    weight         = (weight < _parameters->get_wmin() ? _parameters->get_wmin() : weight);
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
 * \brief    Compute the statistics
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::compute_statistics( void )
{
  _min_x_coord             = 1e+10;
  _mean_x_coord            = 0.0;
  _max_x_coord             = 0.0;
  _min_y_coord             = 1e+10;
  _mean_y_coord            = 0.0;
  _max_y_coord             = 0.0;
  _min_weights_sum         = 1e+10;
  _mean_weights_sum        = 0.0;
  _max_weights_sum         = 0.0;
  _min_population          = 1e+10;
  _mean_population         = 0.0;
  _max_population          = 0.0;
  _min_population_density  = 1e+10;
  _mean_population_density = 0.0;
  _max_population_density  = 0.0;
  _min_road_density        = 1e+10;
  _mean_road_density       = 0.0;
  _max_road_density        = 0.0;
  double n                 = 0.0;
  Node*  node              = get_first();
  while (node != NULL)
  {
    /*** X coordinate ***/
    if (_min_x_coord > node->get_x())
    {
      _min_x_coord = node->get_x();
    }
    if (_max_x_coord < node->get_x())
    {
      _max_x_coord = node->get_x();
    }
    _mean_x_coord += node->get_x();
    
    /*** Y coordinate ***/
    if (_min_y_coord > node->get_y())
    {
      _min_y_coord = node->get_y();
    }
    if (_max_y_coord < node->get_y())
    {
      _max_y_coord = node->get_y();
    }
    _mean_y_coord += node->get_y();
    
    /*** Weight sum ***/
    if (_min_weights_sum > node->get_weights_sum())
    {
      _min_weights_sum = node->get_weights_sum();
    }
    if (_max_weights_sum < node->get_weights_sum())
    {
      _max_weights_sum = node->get_weights_sum();
    }
    _mean_weights_sum += node->get_weights_sum();
    
    /*** Population ***/
    if (_min_population > node->get_population())
    {
      _min_population = node->get_population();
    }
    if (_max_population < node->get_population())
    {
      _max_population = node->get_population();
    }
    _mean_population += node->get_population();
    
    /*** Population density ***/
    if (_min_population_density > node->get_population_density())
    {
      _min_population_density = node->get_population_density();
    }
    if (_max_population_density < node->get_population_density())
    {
      _max_population_density = node->get_population_density();
    }
    _mean_population_density += node->get_population_density();
    
    /*** Road density ***/
    if (_min_road_density > node->get_road_density())
    {
      _min_road_density = node->get_road_density();
    }
    if (_max_road_density < node->get_road_density())
    {
      _max_road_density = node->get_road_density();
    }
    _mean_road_density += node->get_road_density();
    
    /*** Increment n ***/
    n    += 1.0;
    node  = get_next();
  }
  _mean_x_coord            /= n;
  _mean_y_coord            /= n;
  _mean_weights_sum        /= n;
  _mean_population         /= n;
  _mean_population_density /= n;
  _mean_road_density       /= n;
}

/**
 * \brief    Compute the Human activity index
 * \details  --
 * \param    void
 * \return   \e void
 */
void Graph::compute_human_activity_index( void )
{
  Node* node = get_first();
  while (node != NULL)
  {
    if (_parameters->get_human_activity_index())
    {
      node->set_human_activity_index(node->get_population_density()/_max_population_density);
    }
    else
    {
      node->set_human_activity_index(_mean_population_density/_max_population_density);
    }
    node = get_next();
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
  Node* node = get_first();
  while (node != NULL)
  {
    node->reset_state();
    node = get_next();
  }
  _total_log_likelihood         = 0.0;
  _total_log_maximum_likelihood = 0.0;
  _score                        = 0.0;
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

/**
 * \brief    Compute the euclidean distance between two nodes
 * \details  --
 * \param    Node* node1
 * \param    Node* node2
 * \return   \e void
 */
double Graph::compute_euclidean_distance( Node* node1, Node* node2 )
{
  double x1 = node1->get_x();
  double y1 = node1->get_y();
  double x2 = node2->get_x();
  double y2 = node2->get_y();
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

