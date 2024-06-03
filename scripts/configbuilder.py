import argparse

# Created by Jordi Pujol
# Date: 31/05/2024
# Goals: create a Config file to the main program receiving params as arguments]
# Created to be used in irace, the input will not be as readable as possible

'''
Possible params:
-a - algorithm: (G, B, I) - Select the base algorithm. It could be:
    G (Greedy): Non-iterated algorithm. Create a single solution, using a single option to create the next step.
    B (Beam Search): Non-iterated algorithm. Create a group of solutions and return the best one. In each step, expand many possible partial solutions and expand the N-best ones
        - beam-width:      INTEGER - How many solutions will be kept
        - expansion-width: INTEGER - How many new solutions will be seek
    I (iterated): Will use some stopping criteria and another algorithm to create the solution multiple times
        Stopping criterias (The algorithm will stop in the first activated)
            - max-iterations: INTEGER - How many iterations before stop (0=do not use this method)
            - max-no-improv:  INTEGER - How many iterations without improvement before stop (0=do not use this method)
        Algorithm: The same options as above (excluding I, of course)
-p - priority: (G, R, P) - Will config the priority algorithm for the next element. It could be Greedy, Randomized or by Pheromones
    Random params:
    - alpha: [0, 1] - Represents the possibility of get the best element as the next one (1=greedy, 0=full random)
    - k: INTEGER    - Represents the window of elements to be selected as random (1=greedy, 0=all possible elements)
    Pheromones elements:
    - alpha: [0, 1] - Represents the weight of the pheromones in the probability
    - beta:  [0, 1] - Represents the weight of the quality in the probability
    - phi:   [0, 1] - Represents the evaporation rate, i.e.: how much of the pheromone will remain for the next iteration (0=all, 1=nothing)
    - TODO: get all the params

Input examples:
1. Greedy with Randomized(0.8, 5) priority
    input: python configbuild.py -a G -p R 0.8 5
2. Beam-search(3, 4) with Greedy priority
    input: python configbuild.py -a B 3 4 -p G
3. Iterated(2000, 10) Beam-search(2, 3) with Pheromones(0.4, 0.6, 0.9)
    input: python configbuild.py -a I 2000 10 B 2 3 -p P 0.4 0.6 0.9
'''

def create_config_file(filename, algorithm, priority):
    with open(filename, 'w') as file:
        file.write(f'# Algorithm: {algorithm}\n')
        file.write(f'# Priority: {priority}\n')
        file.write('# TODO: Add other parameters\n')

def parse_args():
    parser = argparse.ArgumentParser(description='Config Builder')
    parser.add_argument('-a', '--algorithm', choices=['G', 'B', 'I'], help='Select the base algorithm')
    parser.add_argument('-p', '--priority', choices=['G', 'R', 'P'], help='Config the priority algorithm')
    parser.add_argument('params', nargs='*', help='Additional parameters')
    return parser.parse_args()

def main():
    args = parse_args()
    algorithm = args.algorithm
    priority = args.priority
    filename = '/mnt/c/Users/pujol/source/repos/simple-heuristic/scripts/config.txt'
    create_config_file(filename, algorithm, priority)

if __name__ == '__main__':
    main()