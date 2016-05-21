
/*
 *  ddArray.h
 *
 *  Created by Seung-Chan Kim on 1/28/13.
 *  Created at TCL
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 */

#ifndef DD_ARRAY1_H
#define DD_ARRAY1_H

typedef struct _Array
{
    int *elements;
    int len;
    _Array()
    {
        elements = NULL;
        len = 0;
    }
    _Array(int *data, int len)
    {
        this->elements = new int[len];
        this->len = len;
        for(int i = 0; i < len; i++)
            this->elements[i] = data[i];
    }
    _Array(int len)
    {
        this->len = len;
        this->elements = new int[len];
    }
 
    ~_Array()
    {
        if(this->elements != NULL)
        {
            delete [] this->elements;
            this->elements = NULL;
            this->len = 0;
        }
    }
 
} Array;
 
inline Array* BubbleSort(Array *p)
{
    for(int i = 1; i < p->len; i++)
    {
        for(int j = 0; j < p->len - i; j++)
        {
            if(p->elements[j] > p->elements[j + 1])
            {
                int temp = p->elements[j];
                p->elements[j] = p->elements[j + 1];
                p->elements[j + 1] = temp;
            }
        }
    }
    return p;
}

inline Array* Find_Unique_Elements(Array *p)
{
    p = BubbleSort(p);
 
    int element = p->elements[0];
    int count = 1;
    for(int i = 1; i < p->len; i++)
    {
        if(element == p->elements[i])
            continue;
        else
        {
            element = p->elements[i];
            count++;
        }
    }
 
    Array *result = new Array(count);
    count = 0;
    element = p->elements[0];
    result->elements[count++] = element;
    for(int i = 1; i < p->len; i++)
    {
        if(element == p->elements[i])
            continue;
        else
        {
            element = p->elements[i];
            result->elements[count++] = element;
        }
    }
    return result;
}
 


/*
http://www.softwareandfinance.com/Visual_CPP/Unique_Elements_Arrays.html
int array1[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                 95, 85, 75, 65, 55, 45, 35, 25, 15, 05,
                 10, 15, 20, 25, 30, 35, 40, 45, 50, 55
};
 
int array2[] = { 15, 25, 35, 45, 55,
                 12, 22, 32, 43, 52,
                 15, 25, 35, 45, 55
};

 Array *a1 = new Array(array1, sizeof(array1) / sizeof(array1[0]));
    Array *a2 = new Array(array2, sizeof(array2) / sizeof(array2[0]));
 
    Array *p1 = Find_Unique_Elements(a1);
    Array *p2 = Find_Unique_Elements(a2);
 
 
    std::cout << "\n\nUnique Sorted Elements in Array1: ";
    for(int i = 0; i < p1->len; i++)
        std::cout << p1->elements[i] << " ";
    std::cout << "\n\nUnique Sorted Elements in Array2: ";
    for(int i = 0; i < p2->len; i++)
        std::cout << p2->elements[i] << " ";
    std::cout << "\n\n";
 
    delete a1, a2, p1, p2;

*/


#endif

