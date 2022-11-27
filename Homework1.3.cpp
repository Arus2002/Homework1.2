#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>



int main(){

	int N;
        int fd[2];
	std::cout << "Vector size :";
	std::cin >> N;
	std::vector<int> vec(N);

	for(int i = 0; i < N; i++){
           vec[i] = rand() % 10;
	   std::cout << vec[i] << " ";
	}
	std::cout << std::endl;

	int M;
	std::cout << "Number of processes : ";
	std::cin >> M;
	std::cout << std::endl;
	std::vector<pid_t> pids(M);

	if(pipe(fd) == -1){
                return 1;
        }

	
        for(int i = 0; i < M; i++){
           pids[i] = fork();
           if(pids[i] == 0){
	      if(close(fd[0]) == -1){
		      return 2;
	      }
	      int sum = 0;
              for(int j = i * N/M; j < (i+1) * N/M; j++){
                sum += vec[j];
              }
	      std::cout <<"Parent PID is:" << getppid() << std::endl;
	      std::cout <<"Child PID is:" << getpid() << std::endl;
	      std::cout <<"Sum from Child" << (i + 1) << " :" << sum << std::endl;
	      std::cout << std::endl;
              write(fd[1], &sum, sizeof(sum));
              if(close(fd[1]) == -1){
		      return 3;
	      }
	      exit(0);
           }
	   
        }

        if(close(fd[1]) == -1){
		return 4;
	}
	int result = 0;
        for(int i = 0; i < M; i++){
              wait(NULL);
	      int number;
	      read(fd[0], &number, sizeof(number));
	      result += number;
	}

	if(close(fd[0]) == -1){
		return 5;
	}

	std::cout << "Sum is :" << result << std::endl;
	

}
