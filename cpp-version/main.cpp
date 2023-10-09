// A clone of cmatrix.
#include <iostream>
#include <chrono>
#include <thread>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <random>


class MyMatrix{
    public:
        int rowLength;
        int columnLength;
        int program_speed;
        int space_density;
        int zero_density {250};

        std::vector<std::vector<char>> matrix;
        std::string characters;
        std::string color_reset_code {"\033[0m"};
        std::string color_code {"\033[32m"};

        MyMatrix(){
            // Initial row and column lengths....
            rowLength = 0;
            columnLength = 0;
            program_speed = 100;
            // Initializing characters to choose from.
            characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_-+=[]{}|;:'\",.<>?/ ";
            for(int i = 0; i<zero_density; i++){
                characters.push_back(' ');
            }
            // setting green as default text-color.
            std::cout << color_code;
        }

        ~MyMatrix(){
            /*To reset asci color code*/
            std::cout<<color_reset_code;
        }

        std::vector<char> giveRandomChars(int columnLength){
            std::vector<char>v;
            // radom_device will use hardware signals  and other signal for true random values.
            std::random_device rd;
            // for generating random seed.
            std::mt19937 gen(rd());
            // range of values to generate
            std::uniform_int_distribution<int> distribution(0, characters.size() - 1);
            // selecting random characters of required column length.
            for(int i = 0; i<columnLength; i++)
                v.push_back(characters[distribution(gen)]);
            return v;
        }

        
        std::pair<int, int> getScreenSize(){
            /* if everything ok return terminal size else 
               return {0, 0} as row and column size.
               This will be used update rendered matrix's size
            */
            struct winsize size;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == 0){
                return {size.ws_row, size.ws_col};
            }else{
                throw std::runtime_error("Failed to retrieve terminal size.")
            }
        }

        void setMatrix(){
            /* this will update matrix size if terminal size changes. else it will return*/
            auto tSize = getScreenSize();
            
            // if matrix is rendered correctly and it close to terminal size then return
            if((tSize.second - 2 * columnLength)< 2 && tSize.first == rowLength+1   )
                return;

            // update the column and row length if terminal size is different.
            columnLength = tSize.second / 2;
            rowLength = tSize.first - 1;
            // clear the matrix to rerender on screen.
            matrix.clear();
            for(int i = 0; i<rowLength; i++){
                std::vector<char> row(columnLength, ' ');
                matrix.push_back(row);
            }
        }

        void moveCharacters(){
            /* This function implements falling characters behaviour.
               Here logic is to generate a random column
               set that random colum as matrix[i] and keep setting ith row value to (i+1)th row.
               Doing this will simulate falling character each time matrix is re-rendered*/
            std::vector<char> newChars  = giveRandomChars(columnLength);
            for(int i = 0; i<rowLength; i++){
                for(int j = 0; j<columnLength; j++){
                    char value = matrix[i][j];
                    matrix[i][j] = newChars[j];
                    newChars[j] = value;
                }
            }
        }


        void draw(){
            /* render the matrix to the terminal  */
            for(int i = 0; i<rowLength; i++){
                for(int j = 0; j<columnLength; j++){
                    std::cout<<matrix[i][j]<<" ";
                }
                std::cout<<"\n";
            }
        }

        void clear(int threshold){
            /* keep cleaning the screen after waiting for certain threshold.
            Threshold value is used to control speed of falling characters*/
            std::this_thread::sleep_for(std::chrono::milliseconds(threshold));
            std::system("clear");
        }
};


int main(){
    MyMatrix m1 = MyMatrix();
    m1.clear(0);
    while(true){
        /* 1. Keep matching matrix with terminal size.
           2. Update the matrix to move ith row to the (i+1)th position.
           3. Render matrix to screen 
           4. Finally clear the terminal after waiting for 100 seconds.*/
        m1.setMatrix();
        m1.moveCharacters();
        m1.draw();
        m1.clear(m1.program_speed);
    }
}