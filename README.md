# ARFF Network Log Analyzer

This project is an extension of the program developed during evaluation #1 and aims to analyze network log files formatted in ARFF. The program extracts statistics related to DDoS attacks from the network log and creates a blacklist of malicious addresses to feed an iptables firewall.

## Resources

The following resources are essential for the development of this project:

1. **netlog.arff**: This file contains an example of a network log in ARFF format, with attributes and data. The program should be able to process this file.

2. **main.c**: This is the main program file from which the main function should be developed.

3. **arff.c/arff.h**: These files represent the ARFF file processing library. You can use the functions from evaluation #1 here and implement new functions as needed.

4. **log.c/log.h**: These files are responsible for analyzing the validated ARFF network log file. They should implement various functionalities to generate reports and create a blacklist of malicious addresses.

## Requirements

The requirements for this project include:

1. Modify the data structure of ARFF attributes to handle categorical attributes that are now stored as arrays of separate strings.

2. Validate the data section of the ARFF file by checking the correct number of attributes and data compatibility with assigned types.

3. Create source code files (log.c and log.h) to analyze the validated ARFF network log file. Implement functionalities to:

   - Generate a report of all attacks that occurred and their occurrence counts in the dataset.

   - Generate a report of malicious, potentially malicious, and benign source addresses.

   - Generate a report with the average of the average packet size for each type of attack.

   - Generate a blacklist of source addresses considered malicious.

## Report Formats

The reports should follow these formats:

- "R_ATAQUES.txt": Contains lines in the format "attack_name;occurrence_count" for attacks, excluding the "Normal" class.

- "R_ENTIDADES.txt": Contains lines in the format "source_address;classification" for source addresses, classified as benign, potentially malicious, or malicious.

- "R_TAMANHO.txt": Contains lines in the format "attack_name;average_average_size" for the average packet size in each type of attack.

- "BLACKLIST.bl": Contains a list of source addresses considered malicious.

## How to Run

To compile and run the program, follow these steps:

1. Open the terminal.

2. Navigate to the project directory.

3. Compile the source code using the Makefile by running the following command:
```bash
make
```
4. Tests  
```python
python3 Script.py log.c log.h main.c arff.c arff.h
```

5. Run  
```bash
./ddosanalyzer -i <inputfile>.arff [-p -v -a -b -c -d]
```