#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

void computeHistogram(const vector<unsigned char>& image, vector<int>& histogram);
void computeCumulativeHistogram(const vector<int>& histogram, vector<int>& cumulativeHistogram);
void saveHistogramImage(const vector<int>& histogram, const string& outputFileName);

// Define image parameters
// Change these parameters according to the image being tested
const int IMAGE_SIZE = 452 * 307;

// Define histogram parameters
const int HISTOGRAM_SIZE = 256;
const int OUTPUT_IMAGE_SIZE = 256;

int main() {
    // Read the input image
    const char* inputFileName = "test1.raw"; // Change the name of input file here

    // Check if the input file exists
    ifstream inputFile(inputFileName, ios::binary);
    if (!inputFile) {
        cerr << "Error: Could not open input image file." << endl;
        return 1;
    }

    vector<unsigned char> image(IMAGE_SIZE);
    inputFile.read(reinterpret_cast<char*>(image.data()), IMAGE_SIZE);
    inputFile.close();

    // Compute the histogram
    vector<int> histogram;
    computeHistogram(image, histogram);

    // Compute the cumulative histogram
    vector<int> cumulativeHistogram;
    computeCumulativeHistogram(histogram, cumulativeHistogram);

    // Save the histogram as a 256x256 raw image
    saveHistogramImage(histogram, "test1_histogram_output.raw"); // Change the name of output file here

    return 0;
}

// Function to compute the histogram of an image
void computeHistogram(const vector<unsigned char>& image, vector<int>& histogram) {
    histogram.assign(HISTOGRAM_SIZE, 0);

    for (unsigned char pixel : image) {
        histogram[pixel]++;
    }
}

// Function to compute the cumulative histogram
void computeCumulativeHistogram(const vector<int>& histogram, vector<int>& cumulativeHistogram) {
    cumulativeHistogram.resize(HISTOGRAM_SIZE);
    cumulativeHistogram[0] = histogram[0];

    for (int i = 1; i < HISTOGRAM_SIZE; ++i) {
        cumulativeHistogram[i] = cumulativeHistogram[i - 1] + histogram[i];
    }
}

// Function to save a histogram as a 256x256 raw image
void saveHistogramImage(const vector<int>& histogram, const string& outputFileName) {
    ofstream outputFile(outputFileName, ios::binary);
    if (!outputFile) {
        cerr << "Error: Could not create output histogram image file." << endl;
        return;
    }

    // Find the maximum frequency to scale the histogram for display
    int maxFrequency = *max_element(histogram.begin(), histogram.end());

    // Create the histogram image data
    vector<unsigned char> histogramImage(OUTPUT_IMAGE_SIZE * OUTPUT_IMAGE_SIZE, 255);

    for (int i = 0; i < HISTOGRAM_SIZE; ++i) {
        int barHeight = (histogram[i] * OUTPUT_IMAGE_SIZE) / maxFrequency;

        // Draw vertical bars in black
        for (int j = OUTPUT_IMAGE_SIZE - 1; j >= OUTPUT_IMAGE_SIZE - barHeight; --j) {
            histogramImage[j * OUTPUT_IMAGE_SIZE + i] = 0;
        }
    }

    // Write the histogram image data to the output file
    outputFile.write(reinterpret_cast<const char*>(histogramImage.data()), OUTPUT_IMAGE_SIZE * OUTPUT_IMAGE_SIZE);
    outputFile.close();

    cout << "Histogram image saved as " << outputFileName << endl;
}