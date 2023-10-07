# Linux (Replace with CMake?)
sudo apt-get remove libsfml-dev -y
sudo apt-get install libsfml-dev -y
sudo apt-get install libconfig-dev -y

rm -rf libs/
mkdir -p libs/headers
cd libs/headers
wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
