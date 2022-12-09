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

// Adds new track to the playlist
void CSVOperator::addNewTrack(juce::String path)
{

    std::string trackToBeAdded = path.toStdString() + "\n";

    std::ofstream csvFile(".\\Tracks.csv", std::ios::app);

    //Appending the new track path
    csvFile << trackToBeAdded;

    csvFile.close();
}

// Removes the selected track from the playlist
void CSVOperator::removeTrack(int rowNumber)
{
    DBG("Deletion: " << rowNumber);

    std::string line;
    int lineIndex = 0;
    
    std::ifstream originalFile;
    originalFile.open(".\\Tracks.csv", std::ios::app);
    
    std::ofstream tempFile;
    tempFile.open(".\\Temp.csv", std::ios::app);

    while (getline(originalFile, line))
    {
        // Write every line except the line to be deleted
        if (lineIndex != rowNumber)
        {
            tempFile << line << std::endl; 
        }
        // Increment index
        lineIndex++;
    }

    // Delete the original file and replace it with the new one
    tempFile.close();
    originalFile.close();
    remove(".\\Tracks.csv");
    rename(".\\Temp.csv", ".\\Tracks.csv");
}



