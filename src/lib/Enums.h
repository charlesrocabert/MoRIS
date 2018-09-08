/**
 * \file      Enums.h
 * \author    Charles Rocabert, Jérôme Gippet, Serge Fenet
 * \date      15-12-2014
 * \copyright MoRIS. Copyright (c) 2014-2018 Charles Rocabert, Jérôme Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Definition of enumerations
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

#ifndef __MoRIS__Enums__
#define __MoRIS__Enums__


/**
 * \brief   Type of experimental data
 * \details --
 */
enum type_of_data
{
  PRESENCE_ONLY    = 0, /*!< Presence-only data    */
  PRESENCE_ABSENCE = 1  /*!< Presence-absence data */
};

/**
 * \brief   Jump distribution law
 * \details --
 */
enum jump_distribution_law
{
  DIRAC     = 0, /*!< Dirac law     */
  GAUSSIAN  = 1, /*!< Gaussian law  */
  LOGNORMAL = 2  /*!< Lognormal law */
};


#endif /* defined(__MoRIS__Enums__) */
