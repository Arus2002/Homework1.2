#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>



int main(){

	int N;
        int fd1[2];
	int fd2[2];
	int fd3[2];
	std::cout << "Vector size :";
	std::cin >> N;
	std::vector<int> vec(N);

	srand(wait(NULL));
	for(int i = 0; i < N; i++){
           vec[i] = rand() % 10;
	   std::cout << vec[i] << " ";
	}
	std::cout << std::endl;

	int M;
	std::cout << "Number of processes : ";
	std::cin >> M;
	std::cout << std::endl;
	pid_t pids;

	if(pipe(fd1) == -1){
                return 1;
        }
	

	for(int i = 0; i < M; i++){
        pids = fork();
        if(pids == 0){
	   if(close(fd1[0]) == -1){
	      return 2;
	   }
	   int sum = 0;
	   int start_index = i * N/M;
	   int end_index = (i + 1) * N/M;
	   close(fd2[1]);
	   read(fd2[0] , &start_index , sizeof(start_index));
	   close(fd2[0]);
	   close(fd3[1]);
	   read(fd3[0] , &end_index , sizeof(end_index));
	   close(fd3[0]);
	   std::cout << "Start index is:"<< start_index << " end index is:" << end_index << std::endl;
           for(int j = start_index ; j < end_index; j++){
              sum += vec[j];
           }
	      std::cout <<"Parent PID is:" << getppid() << std::endl;
	      std::cout <<"Child PID is:" << getpid() << std::endl;
	      std::cout <<"Sum from Child" << (i + 1) << " :" << sum << std::endl;
	      std::cout << std::endl;
              write(fd1[1], &sum, sizeof(sum));
              	if(close(fd1[1]) == -1){
		      return 3;
	      	}
	      	exit(0);
           }
	   
        }

       

     
	int start_index;
	close(fd2[0]);
	write(fd2[1] , &start_index , sizeof(start_index));
	close(fd2[1]);

	int end_index;
	close(fd3[0]);
	write(fd3[1] , &end_index , sizeof(end_index));
	close(fd3[1]);

	if(close(fd1[1]) == -1){
                return 1;
        }
	
	int result = 0;
        for(int i = 0; i < M; i++){
		int start = i * N/M;
                int end = (i + 1) * N/M;
                close(fd2[0]);
                write(fd2[1] , &start , sizeof(start));
                close(fd2[1]);

                close(fd3[0]);
                write(fd3[1] , &end , sizeof(end));
                close(fd3[1]);

                wait(NULL);
	        int number;  
	        read(fd1[0], &number, sizeof(number));
	        result += number;
	}

	if(close(fd1[0]) == -1){
		return 5;
	}

	std::cout << "Sum is :" << result << std::endl;
	

}
