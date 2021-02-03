# Exam_Rank_06

### Commands
- memory leaks: valgrind --leak-check=full ./a.out 8084
- sockets leaks: lsof -c a.out | grep TCP
- close socket port: fuser -k [PORT]/tcp
