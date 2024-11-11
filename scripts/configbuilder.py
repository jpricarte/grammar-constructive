import argparse
import json

# Created by Jordi Pujol
# Date: 31/05/2024
# Goals: create a Config file to the main program receiving params as arguments]
# Created to be used in irace, the input will not be as readable as possible

'''
Params:
1. algorithm: (G, B, I, A) - Select the base algorithm. It could be:
    G (Greedy): Non-iterated algorithm. Create a single solution, using a single option to create the next step.
    B (Beam Search): Non-iterated algorithm. Create a group of solutions and return the best one. In each step, expand many possible partial solutions and expand the N-best ones
        - beam-width:      INTEGER - How many solutions will be kept
        - expansion-width: INTEGER - How many new solutions will be seek
    I (iterated): Will use some stopping criteria and another algorithm to create the solution multiple times
        Stopping criterias (The algorithm will stop in the first activated)
            - num-solutions:  INTEGER - How many solutions will be generated in each iteration
            - max-iterations: INTEGER - How many iterations before stop (0=do not use this method)
            - max-no-improv:  INTEGER - How many iterations without improvement before stop (0=do not use this method)
        Algorithm: The same options as above (excluding I, of course)
2. Priority: (G, R, W, Ph, Pi) - Will config the priority algorithm for the next element. It could be Greedy, Randomized, by Pheromones or Pilot method
    Random params:
    - alpha: [0, 1] - Represents the possibility of get the best element as the next one (1=greedy, 0=full random)
    - k: INTEGER    - Represents the window of elements to be selected as random (1=greedy, 0=all possible elements)
    Pheromones elements:
    - alpha: [0, 1] - Represents the weight of the pheromones in the probability
    - beta:  [0, 1] - Represents the weight of the quality in the probability
    - phi:   [0, 1] - Represents the evaporation rate, i.e.: how much of the pheromone will remain for the next iteration (0=all, 1=nothing)
    Pilot params:
    - k: INTEGER    - Represents the depth of the pilot method (0=go all the way down)

Input examples:
1. Greedy with Randomized(0.8, 5) priority
    input: python configbuild.py -a G -p R 0.8 5
2. Beam-search(3, 4) with Greedy priority
    input: python configbuild.py -a B 3 4 -p G 
3. Iterated(2000, 10) Beam-search(2, 3) with Pheromones(0.4, 0.6, 0.9)
    input: python configbuild.py -a I 2000 10 -i B 2 3 -p Ph 0.4 0.6 0.9
'''


def parse_greedy_algorithm(args_list):
    if len(args_list) != 1:
        raise argparse.ArgumentError(None, 'Greedy algorithm usage: G')
    return {"type": "greedy"}


def parse_beam_search_algorithm(args_list):
    if len(args_list) != 3:
        raise argparse.ArgumentError(None, 'Beam search usage: B <beam_width> <expansion_width>')
    beam_width = int(args_list[1])
    expansion_width = int(args_list[2])
    return {"type": "beamsearch", "beam-width": beam_width, "expansion-width": expansion_width}


def parse_internal_algorithm(args):
    if args.internal_algorithm[0] == 'G':
        return parse_greedy_algorithm(args.internal_algorithm)
    elif args.internal_algorithm[0] == 'B':
        return parse_beam_search_algorithm(args.internal_algorithm)
    else:
        argparse.ArgumentError(None, 'The internal algorithm must be G or B')

def parse_iterated_algorithm(args):
    if args.internal_algorithm == None:
        raise argparse.ArgumentError(None, 'It must use the internal algorithm (-i) option')
    if len(args.algorithm) < 3:
        raise argparse.ArgumentError(None, 'Iterated usage: I <max_iter> <max_no_improv> -i [options]')
    max_iter = int(args.algorithm[1])
    max_no_improv = int(args.algorithm[2])
    stop_criteria = {"max-iterations": max_iter, "max-no-improvement-iterations": max_no_improv}
    internal_algorithm = parse_internal_algorithm(args)
    algorithm = {"type": "iterated", "internal-algorithm": internal_algorithm, "stop": stop_criteria}
    if len(args.algorithm) == 4:
        num_solutions = args.algorithm[3]
        algorithm["num-solutions"] = int(num_solutions)
    return algorithm


def parse_algorithm(args):
    if args.algorithm[0] == 'G':
        return parse_greedy_algorithm(args.algorithm), False
    elif args.algorithm[0] == 'B':
        return parse_beam_search_algorithm(args.algorithm), False
    elif args.algorithm[0] == 'I':
        return parse_iterated_algorithm(args), True
    else:
        raise argparse.ArgumentError(None, 'The algorithm must be G, B or I')
    

def parse_greedy_priority(args_list):
    return {"type": "greedy"}


def parse_random_priority(args_list):
    if len(args_list) != 3:
        raise argparse.ArgumentError(None, 'Random priority usage: -p R <alpha> <k>')
    alpha_value = float(args_list[1])
    k_value = int(args_list[2])
    return {"type":  "random", "alpha-value": alpha_value, "k-value": k_value}

def parse_weighted_priority(args_list):
    return {"type": "weighted"}

def parse_pheromone_priority(args_list):
    if len(args_list) != 4:
        raise argparse.ArgumentError(None, 'Pheromone priority usage: -p Ph <alpha> <beta> <phi>')
    alpha_value = float(args_list[1])
    beta_value = float(args_list[2])
    phi_value = float(args_list[3])
    return {"type": "pheromone", "alpha-value": alpha_value, "beta-value": beta_value, "phi-value": phi_value}

def parse_pilot_priority(args_list):
    if len(args_list) != 2:
        raise argparse.ArgumentError(None, 'Pilot priority usage: -p Pi <k>')
    k_value = int(args_list[1])
    return {"type": "pilot", "k-value": k_value}

def parse_priority(args, output, has_internal):
    if args.priority[0] == 'G':
        priority_params = parse_greedy_algorithm(args.priority)
    elif args.priority[0] == 'R':
        priority_params = parse_random_priority(args.priority)
    elif args.priority[0] == 'W':
        priority_params = parse_weighted_priority(args.priority)
    elif args.priority[0] == 'Ph':
        priority_params = parse_pheromone_priority(args.priority)
    elif args.priority[0] == 'Pi':
        priority_params = parse_pilot_priority(args.priority)
    else:
        raise argparse.ArgumentError(None, 'The algorithm must be G, R, W, Ph or Pi')
    
    if (has_internal):
        output["internal-algorithm"]["priority"] = priority_params
    else:
        output["priority"] = priority_params
    return output
 

def main():
    parser = argparse.ArgumentParser(description='Config Builder')
    parser.add_argument('-a', '--algorithm', nargs='+', required=True, help='Select the base algorithm (G, B, I)')
    parser.add_argument('-i', '--internal_algorithm', nargs='+', required=False, help='Select the internal algorithm')
    parser.add_argument('-p', '--priority', nargs='+', required=True, help='Config the priority algorithm for the next element (G, R, W, Ph, Pi)')
    parser.add_argument('-o', '--output', nargs='+', required=False, help='Specify the output filename')

    args = parser.parse_args()
    algorithm, has_internal = parse_algorithm(args)
    output = parse_priority(args, algorithm, has_internal)
    json_output = json.dumps(output, indent=4)

    file = "./"+args.output[0] if args.output else "auto_config.json"

    with open(file, "w") as outfile:
        outfile.write(json_output)


if __name__ == '__main__':
    main()
