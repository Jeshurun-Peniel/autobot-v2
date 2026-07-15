# AUTOBOT
## Installation and Initial Setup

### 1. Prerequisites

AutoBot V2 is developed and tested with:

* **Ubuntu 24.04 LTS**
* **ROS 2 Jazzy Jalisco**
* **Git**
* **colcon** build tools

---

### 2. Install ROS 2 Jazzy

Install **ROS 2 Jazzy Jalisco** by following the official ROS 2 installation guide:

https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html

After installation, source ROS 2:

```bash id="v12agc"
source /opt/ros/jazzy/setup.bash
```

To automatically source ROS 2 whenever a new terminal is opened:

```bash id="myvm86"
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

```bash id="mb8kva"
export ROS_DOMAIN_ID=1
export ROS_LOCALHOST_ONLY=0
```

To make these settings permanent:

```bash id="6d92o8"
echo "export ROS_DOMAIN_ID=1" >> ~/.bashrc
echo "export ROS_LOCALHOST_ONLY=0" >> ~/.bashrc
source ~/.bashrc
```

> **Important:** Configure the same `ROS_DOMAIN_ID` on both the development computer and the Raspberry Pi.

---

## Quick Setup

Once ROS 2 Jazzy is installed, create a workspace and clone the repository:

```bash id="aw8ij4"
mkdir -p ~/autobot_ws/src
cd ~/autobot_ws/src

git clone https://github.com/Jeshurun-Peniel/autobot-v2.git
```

Install the required dependencies:

```bash id="1h8ndj"
cd ~/autobot_ws

rosdep install --from-paths src --ignore-src -r -y
```

Build the workspace:

```bash id="9ytc0w"
colcon build --symlink-install
```

Source the workspace:

```bash id="veoqgi"
source install/setup.bash
```

To automatically source the workspace in every new terminal:

```bash id="n5jdgx"
echo "source ~/autobot_ws/install/setup.bash" >> ~/.bashrc
source ~/.bashrc
```

The **AutoBot V2** workspace is now ready.
