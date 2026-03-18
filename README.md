# StepCounter
Built a step counter using an MPU6050 accelerometer/gyroscope. Logged 6-axis motion to an SD card and sent signals in Python to detect steps. Designed a refined algorithm to handle edge cases such as jumping and noise. 

Samantha Freeland 
Computational Motor Control

# 1. Introduction 
Step counters are common in wearable fitness devices and health monitoring systems. They help users track daily activity levels, assess rehabilitation progress, and encourage physical movement. Most modern devices achieve this by processing motion data from inertial measurement units (IMUs), like accelerometers and gyroscopes, using black-box algorithms or machine learning models. However, these approaches often hide the inner workings of the system and depend on higher computational resources. In this project, I aimed to build a CPU-
based step counter using an Arduino UNO and an MPU-6050 accelerometer/gyroscope. Instead of using pre-built IMU libraries, I processed the raw sensor data manually using basic CPU logic to detect steps in real-time. This project allowed me to explore lightweight signal processing methods suitable for low-power, low-cost devices.

# 2. Literature Review 
Previous work in the field highlights both the strengths and challenges of IMU-based step counting systems. Lucot et al. (2024) presented a step counting method based on deep learning foot flat detection using a shoe-embedded IMU. Their system showed strong generalization across multiple activities like walking, running, and stair climbing. However, the reliance on deep learning and shoe-specific placement makes it impractical for low-power devices or more general applications. While their system achieves robust detection, it requires significant processing resources, which contrasts with my lightweight, CPU-only approach.

Vandermeeren and Steendam (2022) proposed a handheld IMU-based step detection and step length estimation system. Their use of deep learning provides high accuracy without requiring the sensor to be worn on the lower body, making it more user-friendly. However, handheld use introduces variability based on how the user holds the device. Their work provides useful performance benchmarks, though their system’s computational demands exceed what is possible on basic microcontrollers like the Arduino UNO.

Angelucci et al. (2024) focused on designing a flexible IMU-based validation platform for step counting across different scenarios. While they did not go in-depth on algorithmic complexity, they highlighted the importance of consistent validation procedures and hardware flexibility. This inspired me to incorporate basic data logging and real-world testing in my project to evaluate performance in a repeatable way.

# 3. System Design and Implementation 
I used an MPU-6050 sensor to measure acceleration and angular velocity in six axes (X, Y, Z acceleration and rotation). The sensor was connected to an Arduino UNO, and I developed a threshold-based step detection algorithm that relied only on raw Z-axis acceleration data.

// insert photo
# Hardware Setup: 
• Arduino UNO
• MPU-6050 Accelerometer/Gyroscope
• I2C Wiring (SCL/SDA connections with pull-up resistors)
• LCD Display (to show live step count)
• Power and ground connections

The algorithm continuously read acceleration data, converted it to gravitational units (g), and detected steps when the Z-axis value exceeded a predefined threshold. Each valid threshold crossing was counted as a step, with a small time-based debounce to reduce false positives. I also logged acceleration and angular velocity data to a file for later analysis. Example plots of the data are shown below.
// inset photos of graphs 
Spikes in the Z-axis acceleration aligned with physical steps or impacts when the sensor was moved vertically. Angular velocity data provided additional insight into rotational motion, though my algorithm focused on acceleration only.

# 4. Results and Discussion 
The CPU-only step counter successfully detected steps in real-time on the Arduino UNO. During walking trials, the system correctly identified most steps, and the LCD displayed a live step count. However, I noticed that false positives occurred when the sensor was shaken or held still but vibrated slightly, highlighting the need for better filtering or multi-axis analysis. While my method lacks the accuracy and robustness of deep learning systems like those described by Lucot et al. (2024) or Vandermeeren and Steendam (2022), it demonstrates that low-resource step detection is achievable using basic signal processing. Angelucci et al. (2024) emphasized validation, and my logging approach allowed me to qualitatively assess detection performance by comparing step events with the raw data graphs.

# 5. Future Work 
In the future, I would like to:
• Implement moving average filtering to reduce noise
• Expand detection to use multi-axis data and rotational cues
• Explore activity classification beyond walking
• Validate the system with different users and motion profiles
• Compare performance against IMU library-based methods or machine learning
approaches

# 6. Conclusion 
This project demonstrated that real-time step detection can be achieved using only raw accelerometer data and simple CPU logic on a low-power Arduino UNO. While the system has limitations in accuracy compared to more advanced methods, it provides a transparent, explainable, and resource-efficient solution for basic step counting.

# References 
References
1. Lucot, Quentin, et al. “Multi-Activity Step Counting Algorithm Using Deep Learning
Foot Flat Detection with an IMU inside the Sole of a Shoe.” Sensors (Basel,
Switzerland), U.S. National Library of Medicine, 29 Oct. 2024,
pmc.ncbi.nlm.nih.gov/articles/PMC11548453/.
2. Vandermeeren, S., & Steendam, H. (2022). Deep-learning-based step detection and step
length estimation with a handheld IMU. IEEE Sensors Journal, 22(24), 24205-
24221.https://doi.org/10.1109/JSEN.2022.3219412
3. Angelucci, A., Barbieri, L., Caramaschi, S., Sarmiento, C. A., Sekules, V., & Aliverti, A.
(2024, September). A Flexible IMU-Based Unit for Validation Studies: A Step Counting
Application. In International Conference on Pervasive Computing Technologies for
Healthcare (pp. 190-200). Cham: Springer Nature Switzerland.
