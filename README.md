# Off-Line-Signature-Verification-System
Off-Line Signature Verification System using Euclidean Distance
System is initially trained with a set of sample signatures before proceeding to the verification. It involves three steps - First step is the Pre-processing; image is converted into a binary image. Second step is the Feature Extraction; 23 features are extracted from each binary image. Third step is the Template Creation; all the features are combined to form our samples template for comparison with the test signature template. Test template is verified against samples template using the Euclidean distance. Global threshold or local thresholds are used to obtain the accuracy of the system.
Technologies: C++, OpenCV, File Systems
