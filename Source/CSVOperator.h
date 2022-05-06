#pragma once

#include "JuceHeader.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>



class CSVOperator
{
	public:
		CSVOperator();

		// Returns trackPaths array
		static std::vector<std::string> returnTrackPathsArray();

		// Adds a new Track to the CSV file
		static void addNewTrack(juce::String path);

	private:

		// Reads the tracks CSV file and adds them to trackPaths Array
		static std::vector<std::string> readTracksCSV();


		
};

