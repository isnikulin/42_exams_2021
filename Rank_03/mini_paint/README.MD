## Exam Rank 03 -> mini_paint

Key part is the is_in_shape function:

```
    static int is_in_shape(float x, float y, t_shape *tmp)
    {
        float e = 1.00000000;
        float dist = sqrtf(powf(x - tmp->x, 2.00000000) + powf(y - tmp->y, 2.00000000));
    
        if (dist <= tmp->r) {
            if (tmp->r - dist < e)
                return (2);
            return (1);
        }
        return (0);
    }
```
Basically, we assess if current pixel coordinates are in shape (return 1) and then check if the coordinates are on the borders of the circle (return 2).

> :warning: **Do not be alarmed by the formula for finding dist**: it is provided in the subject itself.
> 
You can compare results of your code with results of 'original' (our_mini_paint.c) using test.sh (it runs tests from test folder)