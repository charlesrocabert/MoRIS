/**
 * \file      Prng.cpp
 * \author    Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
 * \date      07-12-2014
 * \copyright MoRIS. Copyright © 2014-2024 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Prng class definition
 */

/****************************************************************************
 * MoRIS (Model of Routes of Invasive Spread)
 * Copyright © 2014-2024 Charles Rocabert, Jérôme M.W. Gippet, Serge Fenet
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

#include "Prng.h"


/*----------------------------
 * CONSTRUCTORS
 *----------------------------*/

/**
 * \brief    Default constructor
 * \details  --
 * \param    void
 * \return   \e void
 */
Prng::Prng( void )
{
  _prng = gsl_rng_alloc(gsl_rng_mt19937);
}

/**
 * \brief    Constructor with seed
 * \details  Initialize constructor with a specified seed
 * \param    unsigned long int seed
 * \return   \e void
 */
Prng::Prng( unsigned long int seed )
{
  _prng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(_prng, seed);
}

/**
 * \brief    Copy constructor
 * \details  --
 * \param    const Prng& prng
 * \return   \e void
 */
Prng::Prng( const Prng& prng )
{
  _prng = gsl_rng_clone(prng._prng);
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
Prng::~Prng( void )
{
  gsl_rng_free(_prng);
}

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Returns a random variate from the uniform distribution in [0, 1[
 * \details  --
 * \param    void
 * \return   \e double
 */
double Prng::uniform( void )
{
  return gsl_ran_flat(_prng, 0.0, 1.0);
}

/**
 * \brief    Returns a random integer variate from the uniform distribution in [min, max]
 * \details  --
 * \param    int min
 * \param    int max
 * \return   \e int
 */
int Prng::uniform( int min, int max )
{
  assert(min <= max);
  return floor(gsl_ran_flat(_prng, min, max+1));
}

/**
 * \brief    Returns a Bernouilli trial with probability p
 * \details  --
 * \param    double p
 * \return   \e int
 */
int Prng::bernouilli( double p )
{
  assert(p >= 0.0);
  assert(p <= 1.0);
  return gsl_ran_bernoulli(_prng, p);
}

/**
 * \brief    Returns a random integer from the binomial distribution, the number of successes in n independent trials with probability p
 * \details  --
 * \param    int n
 * \param    double p
 * \return   \e int
 */
int Prng::binomial( int n, double p )
{
  assert(n >= 0);
  assert(p >= 0.0);
  assert(p <= 1.0);
  return gsl_ran_binomial(_prng, p, n);
}

/**
 * \brief    Compute a random sample n[] from the multinomial distribution formed by N trials from an underlying distribution p[K]
 * \details  --
 * \param    unsigned int* draws
 * \param    double* probas
 * \param    int N
 * \param    int K
 * \return   \e void
 */
void Prng::multinomial( unsigned int* draws, double* probas, int N, int K )
{
  return gsl_ran_multinomial(_prng, K, N, probas, draws);
}

/**
 * \brief Returns a Gaussian random variate, with mean mu and standard deviation sigma
 * \details  --
 * \param    double mu
 * \param    double sigma
 * \return   \e double
 */
double Prng::gaussian( double mu, double sigma )
{
  assert(sigma >= 0.0);
  if (sigma > 0.0)
  {
    return mu+gsl_ran_gaussian_ziggurat(_prng, sigma);
  }
  return mu;
}

/**
 * \brief Returns a random variate from the lognormal distribution, with mean mu and standard deviation sigma
 * \details  --
 * \param    double mu
 * \param    double sigma
 * \return   \e double
 */
double Prng::lognormal( double mu, double sigma )
{
  assert(sigma > 0.0);
  return gsl_ran_lognormal(_prng, gsl_sf_log(mu), sigma);
}

/**
 * \brief Returns a random variate from the Cauchy distribution with scale parameter gamma
 * \details  --
 * \param    double mu
 * \param    double sigma
 * \return   \e double
 */
double Prng::cauchy( double mu, double gamma )
{
  assert(gamma >= 0.0);
  if (gamma > 0.0)
  {
    return mu+gsl_ran_cauchy(_prng, gamma);
  }
  return mu;
}

/**
 * \brief    Returns a random variate from the exponential distribution with mean mu
 * \details  --
 * \param    double mu
 * \return   \e int
 */
int Prng::exponential( double mu )
{
  return ceil(gsl_ran_exponential(_prng, mu));
}

/**
 * \brief    Returns a random variate from the exponential distribution with mean mu
 * \details  --
 * \param    double lambda
 * \return   \e int
 */
int Prng::poisson( double lambda )
{
  assert(lambda >= 0.0);
  return (int)gsl_ran_poisson(_prng, lambda);
}

/**
 * \brief    Returns the selected index after a roulette wheel selection
 * \details  --
 * \param    double* probas
 * \param    int N
 * \return   \e int
 */
int Prng::roulette_wheel( double* probas, double sum, int N )
{
  double draw = uniform()*sum;
  double total = 0.0;
  for (int i = 0; i < N; i++)
  {
    total += probas[i];
    if (draw < total)
    {
      return i;
    }
  }
  printf("Error in roulette wheel\n");
  exit(EXIT_FAILURE);
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/
