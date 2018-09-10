/**
 * \file      Parameters.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      15-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Parameters class declaration
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

#ifndef __MoRIS__Parameters__
#define __MoRIS__Parameters__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>
#include <assert.h>

#include "Prng.h"
#include "Enums.h"


class Parameters
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Parameters( void );
  Parameters( const Parameters& params );
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Parameters( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  inline Prng*             get_prng( void );
  inline unsigned long int get_prng_seed( void ) const;
  
  /*------------------------------------------------------------------ Type of experimental data */
  
  inline type_of_data get_type_of_data( void ) const;
  
  /*------------------------------------------------------------------ Input data filenames */
  
  inline std::string get_map_filename( void ) const;
  inline std::string get_network_filename( void ) const;
  inline std::string get_sample_filename( void ) const;
  
  /*------------------------------------------------------------------ Number of repetitions by simulation */
  
  inline int get_repetitions_by_simulation( void ) const;
  
  /*------------------------------------------------------------------ Number of iterations by simulation */
  
  inline int get_number_of_iterations( void ) const;
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  inline std::pair<double, double>* get_introduction_coordinates( void );
  inline double                     get_lambda( void ) const;
  inline double                     get_mu( void ) const;
  inline double                     get_sigma( void ) const;
  inline jump_distribution_law      get_jump_law( void ) const;
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  inline std::vector<double>* get_road_linear_combination( void );
  
  /*------------------------------------------------------------------ Minimal connectivity of the connectivity graph */
  
  inline double get_minimal_connectivity( void ) const;
  
  /*------------------------------------------------------------------ Extra statistics */
  
  inline bool get_save_outputs( void ) const;
  inline bool saveOutputs( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Parameters& operator=(const Parameters&) = delete;
  
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  inline void set_prng_seed( unsigned long int seed );
  
  /*------------------------------------------------------------------ Type of experimental data */
  
  inline void set_type_of_data( type_of_data data );
  
  /*------------------------------------------------------------------ Input data filenames */
  
  inline void set_map_filename( std::string filename );
  inline void set_network_filename( std::string filename );
  inline void set_sample_filename( std::string filename );
  
  /*------------------------------------------------------------------ Number of repetitions by simulation */
  
  inline void set_repetitions_by_simulation( int repetitions_by_simulation );
  
  /*------------------------------------------------------------------ Number of iterations by simulation */
  
  inline void set_number_of_iterations( int number_of_iterations );
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  inline void set_introduction_coordinates( std::pair<double, double>* introduction_coordinates );
  inline void set_lambda( double lambda );
  inline void set_mu( double mu );
  inline void set_sigma( double sigma );
  inline void set_jump_law( jump_distribution_law jump_law );
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  inline void set_road_linear_combination( std::vector<double>* road_linear_combination );
  
  /*------------------------------------------------------------------ Minimal connectivity of the connectivity graph */
  
  inline void set_minimal_connectivity( double minimal_connectivity );
  
  /*------------------------------------------------------------------ Extra statistics */
  
  inline void set_save_outputs( bool save_outputs );
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void write_parameters( std::string filename );
  
  /*----------------------------
   * PUBLIC ATTRIBUTES
   *----------------------------*/
  
protected:
  
  /*----------------------------
   * PROTECTED METHODS
   *----------------------------*/
  
  /*----------------------------
   * PROTECTED ATTRIBUTES
   *----------------------------*/
  
  /*------------------------------------------------------------------ Pseudorandom numbers generator */
  
  Prng*             _prng; /*!< Pseudorandom numbers generator */
  unsigned long int _seed; /*!< Prng seed                      */
  
  /*------------------------------------------------------------------ Type of experimental data */
  
  type_of_data _data; /*!< Type of experimental data (presence-only or presence-absence) */
  
  /*------------------------------------------------------------------ Data filenames */
  
  std::string _map_filename;     /*!< Map filename     */
  std::string _network_filename; /*!< Network filename */
  std::string _sample_filename;  /*!< Sample filename  */
  
  /*------------------------------------------------------------------ Number of repetitions by simulation */
  
  int _repetitions_by_simulation; /*!< Number of repetitions by simulation */
  
  /*------------------------------------------------------------------ Number of cycles by simulation */
  
  int _number_of_iterations; /*!< Number of iterations by simulation */
  
  /*------------------------------------------------------------------ Simulation parameters */
  
  std::pair<double, double> _introduction_coordinates; /*!< Coordinates of the introduction node */
  double                    _lambda;                   /*!< Lambda                               */
  double                    _mu;                       /*!< Mu                                   */
  double                    _sigma;                    /*!< Sigma                                */
  jump_distribution_law     _jump_law;                 /*!< Jump distribution law                */
  
  /*------------------------------------------------------------------ Linear combination of road categories */
  
  std::vector<double> _road_linear_combination; /*!< Linear combination of the road categories */
  
  /*------------------------------------------------------------------ Minimal connectivity of the connectivity graph */
  
  double _minimal_connectivity; /*!< Minimal connectivity between cells */
  
  /*------------------------------------------------------------------ Extra statistics */
  
  bool _save_outputs; /*!< Save simulation outputs */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Pseudorandom numbers generator */

/**
 * \brief    Get the pseudorandom numbers generator
 * \details  --
 * \param    void
 * \return   \e Prng*
 */
inline Prng* Parameters::get_prng( void )
{
  return _prng;
}

/**
 * \brief    Get the prng seed
 * \details  --
 * \param    void
 * \return   \e Prng*
 */
inline unsigned long int Parameters::get_prng_seed( void ) const
{
  return _seed;
}

/*------------------------------------------------------------------ Type of experimental data */

/**
 * \brief    Get the type of experimental data
 * \details  --
 * \param    void
 * \return   \e type_of_data
 */
inline type_of_data Parameters::get_type_of_data( void ) const
{
  return _data;
}

/*------------------------------------------------------------------ Input data filenames */

/**
 * \brief    Get the map filename
 * \details  --
 * \param    void
 * \return   \e std::string
 */
inline std::string Parameters::get_map_filename( void ) const
{
  return _map_filename;
}

/**
 * \brief    Get the network filename
 * \details  --
 * \param    void
 * \return   \e std::string
 */
inline std::string Parameters::get_network_filename( void ) const
{
  return _network_filename;
}

/**
 * \brief    Get the sample filename
 * \details  --
 * \param    void
 * \return   \e std::string
 */
inline std::string Parameters::get_sample_filename( void ) const
{
  return _sample_filename;
}

/*------------------------------------------------------------------ Number of repetitions by simulation */

/**
 * \brief    Get the number of repetitions by simulation
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Parameters::get_repetitions_by_simulation( void ) const
{
  return _repetitions_by_simulation;
}

/*------------------------------------------------------------------ Number of iterations by simulation */

/**
 * \brief    Get the number of iterations
 * \details  --
 * \param    void
 * \return   \e int
 */
inline int Parameters::get_number_of_iterations( void ) const
{
  return _number_of_iterations;
}

/*------------------------------------------------------------------ Simulation parameters */

/**
 * \brief    Get the coordinates of the introduction node
 * \details  --
 * \param    void
 * \return   \e std::pair<double, double>*
 */
inline std::pair<double, double>* Parameters::get_introduction_coordinates( void )
{
  return &_introduction_coordinates;
}

/**
 * \brief    Get lambda parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_lambda( void ) const
{
  return _lambda;
}

/**
 * \brief    Get mu parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_mu( void ) const
{
  return _mu;
}

/**
 * \brief    Get sigma parameter
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_sigma( void ) const
{
  return _sigma;
}

/**
 * \brief    Get jump distribution law
 * \details  --
 * \param    void
 * \return   \e distribution_law
 */
inline jump_distribution_law Parameters::get_jump_law( void ) const
{
  return _jump_law;
}

/*------------------------------------------------------------------ Linear combination of road categories */

/**
 * \brief    Get the road categories linear combination
 * \details  --
 * \param    void
 * \return   \e std::vector<double>*
 */
inline std::vector<double>* Parameters::get_road_linear_combination( void )
{
  return &_road_linear_combination;
}

/*------------------------------------------------------------------ Minimal connectivity of the connectivity graph */

/**
 * \brief    Get the minimal connectivity between cells
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Parameters::get_minimal_connectivity( void ) const
{
  return _minimal_connectivity;
}

/*------------------------------------------------------------------ Extra statistics */

/**
 * \brief    Get the save simulation outputs boolean
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Parameters::get_save_outputs( void ) const
{
  return _save_outputs;
}

/**
 * \brief    Save outputs?
 * \details  --
 * \param    void
 * \return   \e bool
 */
inline bool Parameters::saveOutputs( void ) const
{
  return _save_outputs;
}

/*----------------------------
 * SETTERS
 *----------------------------*/

/*------------------------------------------------------------------ Pseudorandom numbers generator */

/**
 * \brief    Set the prng seed
 * \details  --
 * \param    unsigned long int seed
 * \return   \e void
 */
inline void Parameters::set_prng_seed( unsigned long int seed )
{
  _seed = seed;
  _prng->set_seed(seed);
}

/*------------------------------------------------------------------ Type of experimental data */

/**
 * \brief    Set the type of experimental data
 * \details  --
 * \param    type_of_data data
 * \return   \e void
 */
inline void Parameters::set_type_of_data( type_of_data data )
{
  _data = data;
}

/*------------------------------------------------------------------ Input data filenames */

/**
 * \brief    Set map filename
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
inline void Parameters::set_map_filename( std::string filename )
{
  _map_filename = std::string(filename);
}

/**
 * \brief    Set network filename
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
inline void Parameters::set_network_filename( std::string filename )
{
  _network_filename = std::string(filename);
}

/**
 * \brief    Set sample filename
 * \details  --
 * \param    std::string filename
 * \return   \e void
 */
inline void Parameters::set_sample_filename( std::string filename )
{
  _sample_filename = std::string(filename);
}

/*------------------------------------------------------------------ Number of repetitions by simulation */

/**
 * \brief    Set the number of repetitions by simulation
 * \details  --
 * \param    int repetitions_by_simulation
 * \return   \e void
 */
inline void Parameters::set_repetitions_by_simulation( int repetitions_by_simulation )
{
  assert(repetitions_by_simulation > 0);
  _repetitions_by_simulation = repetitions_by_simulation;
}

/*------------------------------------------------------------------ Number of iterations by simulation */

/**
 * \brief    Set the number of iterations
 * \details  --
 * \param    int number_of_iterations
 * \return   \e void
 */
inline void Parameters::set_number_of_iterations( int number_of_iterations )
{
  assert(number_of_iterations > 0);
  _number_of_iterations = number_of_iterations;
}

/*------------------------------------------------------------------ Simulation parameters */

/**
 * \brief    Set coordinates of the introduction node
 * \details  --
 * \param    std::pair<double, double>* introduction_coordinates
 * \return   \e void
 */
inline void Parameters::set_introduction_coordinates( std::pair<double, double>* introduction_coordinates )
{
  _introduction_coordinates.first  = introduction_coordinates->first;
  _introduction_coordinates.second = introduction_coordinates->second;
}

/**
 * \brief    Set lambda
 * \details  --
 * \param    double lambda
 * \return   \e void
 */
inline void Parameters::set_lambda( double lambda )
{
  assert(lambda >= 0.0);
  _lambda = lambda;
}

/**
 * \brief    Set mu
 * \details  --
 * \param    double mu
 * \return   \e void
 */
inline void Parameters::set_mu( double mu )
{
  assert(mu >= 0.0);
  _mu = mu;
}

/**
 * \brief    Set sigma
 * \details  --
 * \param    double sigma
 * \return   \e void
 */
inline void Parameters::set_sigma( double sigma )
{
  assert(sigma >= 0.0);
  _sigma = sigma;
}

/**
 * \brief    Set jump law
 * \details  --
 * \param    jump_distribution_law jump_law
 * \return   \e void
 */
inline void Parameters::set_jump_law( jump_distribution_law jump_law )
{
  _jump_law = jump_law;
}

/*------------------------------------------------------------------ Linear combination of road categories */

/**
 * \brief    Set road linear combination
 * \details  --
 * \param    std::vector<double>* road_linear_combination
 * \return   \e void
 */
inline void Parameters::set_road_linear_combination( std::vector<double>* road_linear_combination )
{
  _road_linear_combination.clear();
  for (size_t i = 0; i < road_linear_combination->size(); i++)
  {
    _road_linear_combination.push_back(road_linear_combination->at(i));
  }
}

/*------------------------------------------------------------------ Minimal connectivity of the connectivity graph */

/**
 * \brief    Set minimal connectivity
 * \details  --
 * \param    distribution_law jump_law
 * \return   \e void
 */
inline void Parameters::set_minimal_connectivity( double minimal_connectivity )
{
  assert(minimal_connectivity >= 0.0);
  _minimal_connectivity = minimal_connectivity;
}

/*------------------------------------------------------------------ Extra statistics */

/**
 * \brief    Set save simulation outputs state boolean
 * \details  --
 * \param    bool save_outputs
 * \return   \e void
 */
inline void Parameters::set_save_outputs( bool save_outputs )
{
  _save_outputs = save_outputs;
}


#endif /* defined(__MoRIS__Parameters__) */
