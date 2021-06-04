#!/usr/bin/env python3

def print_args(a, b , c, d):
# def print_args(a, b):
# def print_args(a):

  print("Hello World from python")
  print("--- Received {}".format(a))
  print("--- Received {}".format(b))
  print("--- Received {}".format(c))
  print("--- Received {} which is of length {}".format(d, len(d)))

def no_args():

  return bytearray(b'This is a function with no args')
