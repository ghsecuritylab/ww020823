/*
  SHADE implemented by C++ for Special Session & Competition on Real-Parameter Single Objective Optimization at CEC-2013
  
  Version: 1.0   Date: 28/June/2013
  Written by Ryoji Tanabe (rt.ryoji.tanabe@gmail.com)
*/

#include"de.h"

Fitness SHADE::run() {
 cout << setprecision(16);
 initializeParameters();
 setSHADEParameters();

 //population
  vector <Individual> pop;
  //fitness value of the population
  vector <Fitness> fitness(pop_size, 0);

  vector <Individual> children;
  vector <Fitness> children_fitness(pop_size, 0);

  //initialize population
  for (int i = 0; i < pop_size; i++) {
    pop.push_back(makeNewIndividual());
    children.push_back((variable*)malloc(sizeof(variable) * problem_size));
  }

  // evaluate the initial population's fitness values
  evaluatePopulation(pop, fitness);
  //number of fitness evaluations is initialized
  int nfe = pop_size;

  //best-so-far solution
  Individual bsf_solution = (variable*)malloc(sizeof(variable) * problem_size);
  //fitness value of the best-so-far solution
  Fitness bsf_fitness;
  //initialize the best-so-far solution and its fitness value
  setBestSolution(pop, fitness, bsf_solution, bsf_fitness);
  
  //  cout << bsf_fitness - optimum << endl;

  //SHADE parameters
  
  //External archive
  int arc_ind_count = 0;
  int random_selected_arc_ind;
  vector <Individual> archive;
  for (int i = 0; i < arc_size; i++) {
    archive.push_back((variable*)malloc(sizeof(variable) * problem_size));
  }

  // the contents of M_f and M_cr are all initialiezed 0.5
  vector <variable> memory_sf(memory_size, 0.5);
  vector <variable> memory_cr(memory_size, 0.5);
  variable p_var;
  variable p_min = 2.0 / pop_size;

  //index counter
  int memory_pos = 0;

  int random_selected_period;
  variable mu_sf, mu_cr;
  // F_i and CR_i
  variable *pop_sf = (variable*)malloc(sizeof(variable) * pop_size);
  variable *pop_cr = (variable*)malloc(sizeof(variable) * pop_size);

  // number of success parameters
  int num_success_param;
  //S_F and S_{CR}
  vector <variable> success_sf;
  vector <variable> success_cr;
  //delta fitness (|f(x) - f(u)|)
  vector <variable> dif_fitness;

  variable temp_sum_sf;
  variable sum_dif_fitness;
  variable weight;

  // for current-to-pbest/1
  int p_best_ind;
  int p_num;
  int *sorted_array = (int*)malloc(sizeof(int) * pop_size);
  Fitness *temp_fit = (Fitness*)malloc(sizeof(Fitness) * pop_size);

  //main loop
  while (nfe < max_num_evaluations) {
    for (int i = 0; i < pop_size; i++) sorted_array[i] = i;
    for (int i = 0; i < pop_size; i++)  temp_fit[i] = fitness[i];
    sortIndexWithQuickSort(&temp_fit[0], 0, pop_size - 1, sorted_array);
   
    for (int target = 0; target < pop_size; target++) {
      //In each generation, CR_i and F_i used by each individual x_i are generated by first selecting an index r_i randomly from [1, H] 
      random_selected_period = rand() % memory_size;  
      mu_sf = memory_sf[random_selected_period];
      mu_cr = memory_cr[random_selected_period];

      //generate CR_i and repair its value
      pop_cr[target] = gauss(mu_cr, 0.1);
      if (pop_cr[target] > 1) {
	pop_cr[target] = 1;
      }
      else if (pop_cr[target] < 0) {
	pop_cr[target] = 0;
      }

      //generate F_i and repair its value
      do {	
	pop_sf[target] = cauchy_g(mu_sf, 0.1);
      } while (pop_sf[target] <= 0);

      if (pop_sf[target] > 1) {
	pop_sf[target] = 1;
      }

      //generate p_i in current/to/pbest/1 randomly
      p_var = (0.2 - p_min) * randDouble() + p_min;
      p_num = round(pop_size * p_var);

      //p-best individual is randomly selected from the top pop_size *  p_i members
      p_best_ind = sorted_array[rand() % p_num];

      operateCurrentToPBest1BinWithArchive(pop, &children[target][0], target, p_best_ind, pop_sf[target], pop_cr[target], archive, arc_ind_count);
    }

    // evaluate the children's fitness values
    evaluatePopulation(children, children_fitness);
    
    /////////////////////////////////////////////////////////////////////////
    //update the bsf-solution and check the current number of fitness evaluations
    // if the current number of fitness evaluations over the max number of fitness evaluations, the search is terminated
    // So, this program is unconcerned about SHADE algorithm directly
    for (int i = 0; i < pop_size; i++) {
      nfe++;

      //following the rules of CEC 2013 real parameter competition, 
      //if the gap between the error values of the best solution found and the optimal solution was 10^{−8} or smaller,
      //the error was treated as 0
      if ((children_fitness[i] - optimum) < epsilon) {
	children_fitness[i] = optimum;
      }

      if (children_fitness[i] < bsf_fitness) {
  	bsf_fitness = children_fitness[i];
  	for (int j = 0; j < problem_size; j ++) {
  	  bsf_solution[j] = children[i][j];
  	}
      }
 
      // if (nfe % 1000 == 0) {
      // 	//output the error value
      // 	cout << bsf_fitness - optimum << endl; 
      // }

      if (nfe >= max_num_evaluations) {
  	break;
      }
    }
    ////////////////////////////////////////////////////////////////////////////

    num_success_param = 0;

    //generation alternation
    for (int i = 0; i < pop_size; i++) {
      if (children_fitness[i] == fitness[i]) {
  	fitness[i] = children_fitness[i];
  	for (int j = 0; j < problem_size; j ++) {
  	  pop[i][j] = children[i][j];
  	}
      }
      else if (children_fitness[i] < fitness[i]) {
	dif_fitness.push_back(fabs(fitness[i] - children_fitness[i]));
  	fitness[i] = children_fitness[i];
  	for (int j = 0; j < problem_size; j ++) {
  	  pop[i][j] = children[i][j];
  	}

	//successful parameters are preserved in S_F and S_CR
	success_sf.push_back(pop_sf[i]);
	success_cr.push_back(pop_cr[i]);
	num_success_param++;

	//parent vectors x_i which were worse than the trial vectors u_i are preserved
	if (arc_size > 1) { 
	  if (arc_ind_count < arc_size) {
	    for (int j = 0; j < problem_size; j++) {
	      archive[arc_ind_count][j] = pop[i][j];
	    }
	    arc_ind_count++;
	  }
	  //Whenever the size of the archive exceeds, randomly selected elements are deleted to make space for the newly inserted elements
	  else {
	    random_selected_arc_ind = rand() % arc_size;
	    for (int j = 0; j < problem_size; j++) {
	      archive[random_selected_arc_ind][j] = pop[i][j];
	    }
	  }
	}
      }
    }
  
    // if numeber of successful parameters > 0, historical memories are updated 
    if (num_success_param > 0) {      
      memory_sf[memory_pos] = 0;
      memory_cr[memory_pos] = 0;
      sum_dif_fitness = 0;
      temp_sum_sf = 0;
     
      for (int i = 0; i < num_success_param; i++) {
	sum_dif_fitness += dif_fitness[i];
      } 

      for (int i = 0; i < num_success_param; i++) {	
	weight = dif_fitness[i] / sum_dif_fitness;

	//weighted lehmer mean
	memory_sf[memory_pos] += weight * success_sf[i] * success_sf[i];
	temp_sum_sf += weight * success_sf[i];

	//weighted arithmetic mean
	memory_cr[memory_pos] += weight * success_cr[i];
      }

      memory_sf[memory_pos] /= temp_sum_sf;

      //increment the counter
      memory_pos++;
      if (memory_pos >= memory_size) memory_pos = 0;

      //clear out the S_F, S_CR and delta fitness
      success_sf.clear();
      success_cr.clear();
      dif_fitness.clear();
    }
  }

  return bsf_fitness - optimum;
}

void SHADE::operateCurrentToPBest1BinWithArchive(const vector<Individual> &pop, Individual child, int &target, int &p_best_individual, variable &scaling_factor, variable &cross_rate, const vector<Individual> &archive, int &arc_ind_count) {
  int r1, r2;
  
  do {
    r1 = rand() % pop_size;
  } while (r1 == target);
  do {
    r2 = rand() % (pop_size + arc_ind_count);
  } while ((r2 == target) || (r2 == r1));

  int random_variable = rand() % problem_size;
  
  if (r2 >= pop_size) {
    r2 -= pop_size;
    for (int i = 0; i < problem_size; i++) {
      if ((randDouble() < cross_rate) || (i == random_variable)) {
	child[i] = pop[target][i] + scaling_factor * (pop[p_best_individual][i] - pop[target][i]) + scaling_factor * (pop[r1][i] - archive[r2][i]);
      }
      else {
	child[i] = pop[target][i];
      }
    }
  }
  else {
    for (int i = 0; i < problem_size; i++) {
      if ((randDouble() < cross_rate) || (i == random_variable)) {
	child[i] = pop[target][i] + scaling_factor * (pop[p_best_individual][i] - pop[target][i]) + scaling_factor * (pop[r1][i] - pop[r2][i]);
      }
      else {
	child[i] = pop[target][i];
      }
    }
  }

  //If the mutant vector violates bounds, the bound handling method is applied
  modifySolutionWithParentMedium(child,  pop[target]);
}

void  SHADE::setSHADEParameters() {
  arc_size = g_arc_size;
  memory_size = g_memory_size;
}

