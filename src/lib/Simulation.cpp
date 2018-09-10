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
 * \param    Parameters* parameters
 * \return   \e void
 */
Simulation::Simulation( Parameters* parameters )
{
  assert(parameters != NULL);
  _parameters = parameters;
  _graph      = new Graph(_parameters);
  _iteration  = 0;
  if (_parameters->saveOutputs())
  {
    std::ofstream tree_file("lineage_tree.txt", std::ios::out | std::ios::trunc);
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
 * \brief    Compute the next iteration
 * \details  --
 * \param    void
 * \return   \e void
 */
void Simulation::compute_next_iteration( void )
{
  std::ofstream tree_file;
  if (_parameters->saveOutputs())
  {
    tree_file.open("lineage_tree.txt", std::ios::out | std::ios::app);
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 1) For each node of the graph */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  Node*                 start_node = _graph->get_first();
  Prng*                 prng       = _parameters->get_prng();
  double                lambda     = _parameters->get_lambda();
  double                mu         = _parameters->get_mu();
  double                sigma      = _parameters->get_sigma();
  jump_distribution_law jump_law   = _parameters->get_jump_law();
  while (start_node != NULL)
  {
    /*-----------------------------------------------*/
    /* 1.1) For each repetition, run the jump module */
    /*-----------------------------------------------*/
    for (int rep = 0; rep < _parameters->get_repetitions_by_simulation(); rep++)
    {
      /* If the cell is occupied */
      if (start_node->get_current_state(rep) == 1)
      {
        start_node->set_next_state(rep, 1);
        
        /* A) Draw the number of jumps to accomplish */
        int number_of_jumps = prng->poisson(lambda*start_node->get_jump_probability());
        
        /* B) Compute each jump event ---------------*/
        for (int jump = 0; jump < number_of_jumps; jump++)
        {
          std::vector<Node*> tagged_nodes;
          Node*  current_node     = start_node;
          int    current_id       = current_node->get_identifier();
          double distance         = 0.0;
          double current_distance = 0.0;
          if (jump_law == DIRAC)
          {
            distance = mu;
          }
          else if (jump_law == GAUSSIAN)
          {
            distance = prng->gaussian(mu, sigma);
          }
          else if (jump_law == LOGNORMAL)
          {
            distance = pow(10.0, prng->gaussian(log10(mu), sigma));
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
            if (_parameters->saveOutputs())
            {
              double euclidean_dist = compute_euclidean_distance(start_node, current_node);
              tree_file << start_node->get_identifier() << " " << current_node->get_identifier() << " " << distance << " " << euclidean_dist << " " << _iteration << "\n";
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
  if (_parameters->saveOutputs())
  {
    tree_file.close();
  }
  
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /* 2) Update all the cell states */
  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  
  _graph->update_state();
  _iteration++;
}

/**
 * \brief    Compute the optimization score
 * \details  --
 * \param    void
 * \return   \e void
 */
void Simulation::compute_score( void )
{
  _graph->compute_score();
}

/**
 * \brief    Write simulation state
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
void Simulation::write_state( std::string filename )
{
  _graph->write_state(filename);
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

/**
 * \brief    Compute the euclidean distance between two nodes
 * \details  --
 * \param    Node* node1
 * \param    Node* node2
 * \return   \e void
 */
double Simulation::compute_euclidean_distance( Node* node1, Node* node2 )
{
  double x1 = node1->get_x_coord();
  double y1 = node1->get_y_coord();
  double x2 = node2->get_x_coord();
  double y2 = node2->get_y_coord();
  return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

