# Install and Run Arduino IDE on Raspberry Pi

To install the Arduino IDE on Raspberry Pi OS (formerly known as Raspbian), you can use the Terminal. Here are the steps to install Arduino IDE on your Raspberry Pi:

1. **Open the Terminal:**
   Open the Terminal on your Raspberry Pi. You can usually find it in the "Accessories" or "System Tools" menu.

2. **Update and Upgrade:**
   It's a good practice to update the package list and upgrade existing packages to ensure you have the latest software. Run the following commands:
   
   ```
   sudo apt update
   sudo apt upgrade
   ```

3. **Install Arduino IDE:**
   You can install the Arduino IDE using the "apt" package manager. Run the following command:

   ```
   sudo apt install arduino
   ```

   This command will install the Arduino IDE and its dependencies.

4. **Add User to the "dialout" Group:**
   To allow your user to access the serial ports (necessary for uploading code to Arduino boards), you must add your user to the "dialout" group. Replace `<username>` with your actual username in the following command:
   ```
   sudo usermod -a -G dialout <username>
   ```
   If you don't know your username, type the following command in the terminal:
   ```
   whoami
   ```
   The output of the command will be your username.

5. **Launch Arduino IDE:**
   After the installation is complete, you can launch the Arduino IDE by searching for it in the application menu (maybe under the *Electronics* category) or by running the following command in the Terminal:

   ```
   arduino
   ```

   The Arduino IDE should open and be ready for use.

You can now use this IDE to program Arduino boards directly from your Raspberry Pi.