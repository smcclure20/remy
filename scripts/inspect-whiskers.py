#!/bin/bash

import sys
sys.path.insert(0, "/home/eecs/sarah/remy")

from protobufs.dna_pb2 import WhiskerTree

def load_whiskers(whisker_file_name):
    # Read the whiskers in 
    fd = open(whisker_file_name, "rb")
    tree = fd.read()
    whiskertree = WhiskerTree()
    whiskertree.ParseFromString(tree)
    return whiskertree

# Then analyze them however you want
# def analyze(constraint):
#     for whisker

def whisker_slowrecwma_constraint(whisker):
    return not(whisker.domain.upper.slow_rec_rec_ewma >= 0 and whisker.domain.lower.slow_rec_rec_ewma <= 0)

def whisker_rttr_constraint(whisker):
    lower_constraint = whisker.domain.lower.rtt_ratio == 1.0
    upper_constraint = whisker.domain.upper.rtt_ratio == 1.0
    return lower_constraint and upper_constraint

def whisker_intersend_constraint(whisker):
    return whisker.intersend < 0.8 and whisker.domain.upper.rtt_ratio > 2.0


def mem_str(memory):
    return "<sewma={}, recwma={}, rttr={}, slow_recwma={}, loss={}>".format(memory.rec_send_ewma, memory.rec_rec_ewma, memory.rtt_ratio, memory.slow_rec_rec_ewma, memory.recent_loss)

def mem_range_str(memory_range):
    return "(low: {}, hi: {}".format(mem_str(memory_range.lower), mem_str(memory_range.upper))

def whisker_str(whisker):
    return "[{} => ({} + {} * win, pace={})]".format(mem_range_str(whisker.domain), whisker.window_increment, whisker.window_multiple, whisker.intersend)

def print_tree(whiskers):
    if len(whiskers.children) == 0:
        print(whisker_str(whiskers.leaf))
    
    else:
        for child in whiskers.children:
            print_tree(child)

def get_whiskers_for_constraint(whiskertree, constraint):
    all_whiskers = []
    if len(whiskertree.children) == 0:
        if constraint(whiskertree.leaf):
            return [whiskertree.leaf]
        else:
            return []
    
    else:
        for child in whiskertree.children:
            all_whiskers += get_whiskers_for_constraint(child, constraint)
    
    return all_whiskers

def count_whiskers_for_constraint(whiskertree, constraint):
    count = 0
    if len(whiskertree.children) == 0:
        if constraint(whiskertree.leaf):
            return 1
        else:
            return 0
    
    else:
        for child in whiskertree.children:
            count += count_whiskers_for_constraint(child, constraint)
    
    return count

if __name__ == "__main__":
    whisker_file = sys.argv[1]
    whiskertree = load_whiskers(whisker_file)
    whiskers = get_whiskers_for_constraint(whiskertree, whisker_intersend_constraint)
    # total_intersend = 0
    # for whisker in whiskers:
    #     total_intersend += whisker.intersend
        # print(whisker_str(whisker))
    # print(total_intersend / len(whiskers))

    # print(whiskertree.domain.active_axis)

    # count = count_whiskers_for_constraint(whiskertree, whisker_intersend_constraint)
    # print(count)
    
