# AUTOBOT

## Installation and Initial Setup

### 1. Prerequisites

AutoBot V2 is developed and tested with:

* **Ubuntu 24.04 LTS**
* **ROS 2 Jazzy Jalisco**
* **Git**
* **colcon** build tools
* **LibSerial**

---

### 2. Install ROS 2 Jazzy

Install **ROS 2 Jazzy Jalisco** by following the official ROS 2 installation guide:

https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html

After installation, source ROS 2:

```bash
source /opt/ros/jazzy/setup.bash
```

To automatically source ROS 2 whenever a new terminal is opened:

```bash
echo "source /opt/ros/jazzy/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

---

### 3. Configure ROS 2 Network Communication

AutoBot V2 uses ROS 2 communication between the development computer and the Raspberry Pi.

Both devices must:

* Be connected to the same network
* Use the same `ROS_DOMAIN_ID`
* Have `ROS_LOCALHOST_ONLY` disabled

Set the ROS 2 communication parameters:

```bash
export ROS_DOMAIN_ID=1
export ROS_LOCALHOST_ONLY=0
```

To make these settings permanent:

```bash
echo "export ROS_DOMAIN_ID=1" >> ~/.bashrc
echo "export ROS_LOCALHOST_ONLY=0" >> ~/.bashrc
source ~/.bashrc
```

> **Important:** Configure the same `ROS_DOMAIN_ID` on both the development computer and the Raspberry Pi.

---

## Quick Setup

Once ROS 2 Jazzy is installed, install the required system dependencies:

```bash
sudo apt update
sudo apt install -y \
  git \
  python3-colcon-common-extensions \
  python3-rosdep \
  libserial-dev
```

Initialize `rosdep` if it has not already been initialized:

```bash
sudo rosdep init
rosdep update
```

> If `rosdep` has already been initialized, skip `sudo rosdep init` and run only `rosdep update`.

### Clone the Repository

Create a ROS 2 workspace and clone the repository:

```bash
mkdir -p ~/autobot_ws/src
cd ~/autobot_ws/src

git clone https://github.com/Jeshurun-Peniel/autobot-v2.git
```

### Install ROS 2 Dependencies

Move to the workspace root:

```bash
cd ~/autobot_ws
```

Install all required ROS 2 package dependencies:

```bash
rosdep install --from-paths src --ignore-src -r -y
```

### Build the Workspace

```bash
colcon build --symlink-install
```

### Source the Workspace

```bash
source install/setup.bash
```

To automatically source the workspace whenever a new terminal is opened:

```bash
echo "source ~/autobot_ws/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

The **AutoBot V2** workspace is now ready.
