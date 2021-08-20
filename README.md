# SCP
Scheduling with cadence constraints problem

### Description

This problem is inspired by a large multinational automotive company, which imposes cadences when manufacturing their cars, so the ones with more demanding 
features aren't scheduled in close proximity to each other. Hereafter, cars will be denoted as jobs. The goal of this project is to sequence the maximum number of 
consecutive jobs, while respecting their cadences.

Each cadence is described by one of the following policies.

- **Policy 1:** given a subset of jobs *S*, at most *C(S)* jobs in *S* can be scheduled contiguously in the sequence;
- **Policy 2:** given a subset of jobs *T*, all jobs in *T* put in the sequence must have at least *A(T)* jobs outside of *T* in between them.

### Instances

There are four sets of instances: A, B, C and D. Set A was obtained by real demands of the company. The remaining sets had the addition of one extra cadence and 
have different limits on the number of jobs with the same cadence pattern. Set B has at most 8 jobs with the same cadences, if there are enough cadence patterns to 
do so. This limit is 4 and 2 for sets C and D, respectively. Hence there is an increasing difficulty from set A to set D.

### Versions

Different strategies were tested to solve this problem:

- Feasibility checking using binary and iterative searches with trivial, combinatorial (here called "algorithm") and heuristic bounds;
- Pure optimization model.

### Contributors
- Lara di Cavalcanti Pontes
- Carlos Vinícius Costa Neves (https://github.com/cvneves)
