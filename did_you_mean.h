/**
 * This file contains definitions for the functions that provide did-you-mean functionality
 * for the shell.
 *
 */

#include <vector>
#include <string>

#ifndef DID_YOU_MEAN_H
#define DID_YOU_MEAN_H

namespace dym
{
  void all_commands( std::vector<std::string>& commands );

  void near_matches( std::vector<std::string>& commands, std::string line, std::vector<std::string>& near_matches  );

  void display_results( std::vector<std::string>& near_matches );

  void do_you_mean( std::string& line );
};
#endif
