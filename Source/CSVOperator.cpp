#include "CSVOperator.h"

CSVOperator::CSVOperator()
{
    readTracksCSV();
}


std::vector<std::string> CSVOperator::returnTrackPathsArray()
{
    std::vector<std::string> trackPaths = readTracksCSV();
    return trackPaths;
}

// Reads the tracks CSV file and returns them as a vector
std::vector<std::string> CSVOperator::readTracksCSV()
{
    std::string line;
    std::vector<std::string> tracks;

    juce::String filePath = juce::File::getCurrentWorkingDirectory().getFullPathName();
    filePath.toStdString();

    juce::File tracksFile(filePath + "\\Tracks.csv");

    // Checks if there is a CSV file to keep tracks, if not, creates it first
    if (!tracksFile.exists())
    {
        tracksFile.create();
    }

    juce::FileInputStream csvFile(tracksFile);

    if (csvFile.openedOk())
    {
        while (!csvFile.isExhausted())
        {
            try {
                juce::String juceLine = csvFile.readNextLine();

                // readNextLine returns a juce string, so we need to convert it to std::string first
                line = juceLine.toStdString();
                tracks.push_back(line);
            }
            catch (const std::exception& e)
            {
                DBG("CSVOperator::readCSV bad data");
            }
        }
    }

    return tracks;
}

void CSVOperator::addNewTrack(juce::String path)
{

    std::string trackToBeAdded = path.toStdString() + "\n";

    std::ofstream csvFile(".\\Tracks.csv", std::ios::app);

    //Appending the new track path
    csvFile << trackToBeAdded;

    csvFile.close();
}



