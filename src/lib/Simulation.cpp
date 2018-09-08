/**
 * \file      Simulation.cpp
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      16-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Simulation class definition
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

#include "Simulation.h"


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
 * \param    int introduction_node
 * \param    double lambda
 * \param    double mu
 * \param    double sigma
 * \param    jump_distribution_law jump_law
 * \param    int nb_repetitions
 * \param    std::vector<double>* road_linear_combination
 * \param    double minimal_connectivity
 * \param    bool save_lineage_tree
 * \return   \e void
 */
Simulation::Simulation( Prng* prng, type_of_data data, std::string network_filename, std::string map_filename, std::string sample_filename, int introduction_node, double lambda, double mu, double sigma, jump_distribution_law jump_law, int nb_repetitions, std::vector<double>* road_linear_combination, double minimal_connectivity, bool save_lineage_tree )
{
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) Set prng                        */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _prng = prng;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Set number of repetitions       */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _nb_repetitions = nb_repetitions;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Set cells set                   */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _graph = new Graph(prng, data, network_filename, map_filename, sample_filename, nb_repetitions, road_linear_combination, minimal_connectivity);
  _graph->compute_jump_probabilities();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 4) Set n                           */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _n = _graph->get_number_of_nodes();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 5) Set introduction cell           */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _introduction_node = introduction_node;
  _graph->reset_states();
  _graph->get_node(_introduction_node)->set_as_introduction_node();
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 6) Set jump probability parameters */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  assert(lambda >= 0.0);
  assert(mu >= 0.0);
  assert(sigma >= 0.0);
  _lambda   = lambda;
  _mu       = mu;
  _sigma    = sigma;
  _jump_law = jump_law;
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 7) Set other attributes            */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _iteration         = 0;
  _save_lineage_tree = save_lineage_tree;
  if (_save_lineage_tree)
  {
    std::ofstream tree_file("tree.txt", std::ios::out | std::ios::trunc);
    tree_file.close();
  }
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
Simulation::~Simulation( void )
{
  delete _graph;
  _graph = NULL;
}

/**----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Update simulation
 * \details  --
 * \param    bool last_iteration
 * \return   \e void
 */
void Simulation::update( bool last_iteration )
{
  std::ofstream tree_file;
  if (_save_lineage_tree)
  {
    tree_file.open("tree.txt", std::ios::out | std::ios::app);
    tree_file << "start end geodesic_distance iteration\n";
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) For each node of the graph */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  Node* start_node = _graph->get_first();
  while (start_node != NULL)
  {
    /*-----------------------------------------------*/
    /* 1.1) For each repetition, run the jump module */
    /*-----------------------------------------------*/
    for (int rep = 0; rep < _nb_repetitions; rep++)
    {
      /*--------------------------------------------------------------------*/
      /* If the colony is allowed to jump (i.e. it is occupied by a colony) */
      /*--------------------------------------------------------------------*/
      if (start_node->isAllowedToJump(rep))
      {
        /* A) Draw the number of jumps to accomplish */
        int number_of_jumps = _prng->poisson(_lambda*start_node->get_jump_probability());
        
        /* B) Compute each jump event ---------------*/
        for (int jump = 0; jump < number_of_jumps; jump++)
        {
          std::vector<Node*> tagged_nodes;
          Node*  current_node     = start_node;
          int    current_id       = current_node->get_identifier();
          double distance         = 0.0;
          double current_distance = 0.0;
          if (_jump_law == DIRAC)
          {
            distance = _mu;
          }
          else if (_jump_law == GAUSSIAN)
          {
            distance = _prng->gaussian(_mu, _sigma);
          }
          else if (_jump_law == LOGNORMAL)
          {
            distance = _prng->gaussian(log(_mu), _sigma);
          }
          /* While distance is lower than drawn distance, jump and tag explored node */
          while (current_distance < distance)
          {
            current_node->tag();
            tagged_nodes.push_back(current_node);
            current_node = current_node->jump();
            /* If current node is out of the map, stop walking */
            if (current_node == NULL)
            {
              break;
            }
            /* If current node itself, stop walking */
            if (current_node->get_identifier() == current_id)
            {
              break;
            }
            current_distance += 1.0;
          }
          /* If current node is inside the map, set it occupied */
          if (current_node != NULL)
          {
            current_node->set_next_state(rep, 1);
            if (_save_lineage_tree)
            {
              tree_file << start_node->get_identifier() << " " << current_node->get_identifier() << " " << distance << " " << _iteration << "\n";
            }
          }
          /* Untag visited node during this random walk */
          for (size_t i = 0; i < tagged_nodes.size(); i++)
          {
            tagged_nodes[i]->untag();
          }
        }
      }
    }
    
    /*-----------------------------------------------*/
    /* 1.2) Go to the next cell                      */
    /*-----------------------------------------------*/
    start_node = _graph->get_next();
  }
  if (_save_lineage_tree)
  {
    tree_file.close();
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Update all the cell states */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _graph->update(last_iteration);
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 3) Update cycle               */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _iteration++;
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/
