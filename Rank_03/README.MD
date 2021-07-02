## Exam Rank 03

This exam requires you to complete 1 out of 2 very similar projects:

* **micro_paint** [subject](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/subjects/micro_paint.txt) | [solution](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/micro_paint/micro.c)
(print only unique characters encountered in 2 arguments)
* **mini_paint** [subject](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/subjects/mini_paint.txt) | [solution](https://github.com/Preposterone/42_exams_2021/blob/master/Rank_03/mini_paint/mini.c)
(print without doubles, the characters that appear in both arguments, in the order they appear in the first one)
  
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