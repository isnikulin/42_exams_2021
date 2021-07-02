## Exam Rank 03

This exam requires you to complete 1 out of 2 very similar projects:

Read from file and print shapes to consoles:
* **micro_paint** [subject](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/subjects/micro_paint.txt) | [solution](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/micro_paint/micro.c)
(rectangles either filled or empty)
* **mini_paint** [subject](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/subjects/mini_paint.txt) | [solution](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/mini_paint/mini.c)
(circles either filled or empty)
  
### General overview
Since the tasks are so similar, you can code your solution in a very similar fashion, with very minor differences, apart from the algorithm.

It can be structured as follows:
```
* Check args;
* Try to fopen argv[1];
* Try to make canvas (area which will be 'painted' on);
* Try to put shapes on the canvas (mutate canvas object);
* Paint resulting picture;
```