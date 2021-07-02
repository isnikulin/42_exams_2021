## Exam Rank 03 -> micro_paint

Key part is the is_in_shape function:

```
    static int is_in_shape(float x, float y, t_shape *tmp)
    {
        float e = 1.00000000;
    
        if(x < tmp->x || tmp->x + tmp->w < x || y < tmp->y || tmp->y + tmp->h < y)
            return (0);
        if(x - tmp->x < e || tmp->x + tmp->w - x < e || y - tmp->y < e || tmp->y + tmp->h - y < e)
            return (2);
        return (1);
    }
```
Basically, we assess if current pixel coordinates are in shape (return 1) and then check if the coordinates are on the borders of the rectangle (return 2).

You can compare results of your code with results of 'original' (our_micro_paint.c) using test.sh (it runs tests from test folder)