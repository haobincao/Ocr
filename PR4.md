# PR4

# **2. Introduction**

In this programming assignment you will write a program to take a black and white (8-bit grayscale) image containing the bitmap representations of a set of decimal digits 0-9 and attempt to recognize/classify each digit/character. This process is similar to what is known as OCR (Optical Character Recognition) where a document is scanned and converted to text (ASCII) representation for future editing or ease of storage rather than simply be saved as an image. We will restrict the characters you have to recognize to **decimal digits** using a fonts such as Verdana or ???.

Your program will:

- read in **two `.bmp` images** containing digits,
- find and identify blobs of pixels that form a digit,
- perform feature analysis to classify each digit `0``9`,
- output a string of those digits to form a `BigInt`,
- and then output the **sum** and **difference** of those two numbers.

For example, given the two images shown below:

![https://apollo-media.codio.com/media%2F1%2F95bd69f754ef19755d97def4c91b5028-4a48c6e9c22ee444.bmp](https://apollo-media.codio.com/media%2F1%2F95bd69f754ef19755d97def4c91b5028-4a48c6e9c22ee444.bmp)

![https://apollo-media.codio.com/media%2F1%2F452db41d4dd3a0a2f67c032be0c6f982-dd0f435d59b4929f.bmp](https://apollo-media.codio.com/media%2F1%2F452db41d4dd3a0a2f67c032be0c6f982-dd0f435d59b4929f.bmp)

Your program would output:

`86490 + 27531 = 114021
86490 - 27531 = 58959`

While it may seem like a large task we will break it down into smaller pieces. You will utilize the **breadth-first search** (BFS) algorithm utilized in the maze-search project as well as your `BigInt` class from your earlier homework.

### **Learning Goals**

In this project you will practice:

- Use STL class(es) `vector` and `deque`.
- Apply operator overloading to make working with objects easier
- Practice using BFS and understand its applications
- Implement classes (objects) in C++.
- Recognize appropriate places to use C++ references (e.g. `T&`)

### **Video Overview**

Prof. Redekopp has recorded a two part introduction and overview of what you will be doing. We recommend watching these videos after you read through the assignment once **but before you begin coding**.

- [Overview - Part 1](https://ee.usc.edu/~redekopp/Streaming/cs103/pr4-ocr-overview/pr4-ocr-overview.html)
- [Overview - Part 2](https://ee.usc.edu/~redekopp/Streaming/cs103/pr4-ocr-overview-pt2/pr4-ocr-overview-pt2.html)

# ***3. Background***

Just read this whole page to get an idea of things you will implement when you start coding. The actual procedure and order for how you can code this assignment are on the next pages.

## **Images**

We have provided an updated BMP library (used in earlier applications) in `bmplib.h/cpp` that can now read in **arbitrary** dimension images (not just 256x256 images). But since the 2D and 3D arrays are of a variable size (i.e. we don’t know the necessary dimensions in advance), our library uses **dynamic allocation** and returns an `unsigned char**` which is a **pointer to array of pointers** to **rows/1D arrays of pixels**), similar to the 2D arrays you’ve used in past assignments. Thus, we will need to deallocate the image at some point. In addition, our library to read the image will need to output the **height** and **width** of the image so that you can store those values to interpret it correctly.

The prototypes are now:

*`// grayscale functions***uint8_t**** readGSBMP(**const** **char** filename[], **int**& height, **int**& width);
**int** **writeGSBMP**(**const** **char** filename[], **uint8_t** **outputImage,**int** height,**int** width);
*// RGB functions***uint8_t** ***readRGBBMP(**const** **char** filename[], **int**& height,**int**& width);
**int** **writeRGBBMP**(**const** **char** filename[], **uint8_t** ***outputImage,**int** height,**int** width);`

However, we will only use the **grayscale** images/functions.

## **NumImg class**

To model, encapsulate, and abstract operations on the image of digits that represent a number, we’ve created a `NumImg` class. This class will serve several roles:

- “Own” the 2D image array and, thus, be in charge of its deallocation.
- Provide an operation to find the digits (connected components of pixels) and create `DigitBlob` objects (more on `DigitBlob`s later).
- Provide an operation to run feature analysis on the `DigitBlob`s (really on the pixels forming a digit) and classify each as a digit `0``9`.
- Access the resulting digits as a string
- Other accessors to data including the `DigitBlob`s if necessary.

### **Grouping Pixels Into Connected components**

One of the main operations your `NumImg` class should perform is to find the digits by grouping connected pixels into a single “component” (thus implementing what would be termed a “connected component” algorithm). Essentially, you will need to group connected black pixels into a region, defined by a **bounding box**, and eventually abstracted into a `DigitBlob` object.

We can begin by simply scanning through the image **top-to-bottom, left-to-right**. When we find a black pixel, we can then pause and find all connected black pixels which must be a digit.

At the point we should group any neighboring black pixels to that starting pixel and repeat until all connected black pixels have been identified. After a minute of consideration, you should realize that this can be achieved using a **breadth-first search (BFS)**. Rather than using the 4-connected neighbors (N,W,S,E) we will use the **8-connected neighbors (N,S,E,W,NW,SW,SE,NE)**.

![https://apollo-media.codio.com/media%2F1%2Faf99a0c02c1bf982d385d27c3fc1d72c-db7bdd452e67a188.webp](https://apollo-media.codio.com/media%2F1%2Faf99a0c02c1bf982d385d27c3fc1d72c-db7bdd452e67a188.webp)

As we perform the BFS, we would like to find the **bounding box** of the digit by finding the minimum and maximum row and column of any pixel in the image. You will not have to draw the bounding box but we have done so to illustrate the concept.

![https://apollo-media.codio.com/media%2F1%2F45123d677599762574aff79f9459665c-c30d6d827e0c708b.webp](https://apollo-media.codio.com/media%2F1%2F45123d677599762574aff79f9459665c-c30d6d827e0c708b.webp)

To give more detail, consider the image below we may find the black pixel at 1,3 first.

![https://apollo-media.codio.com/media%2F1%2F61868b3b9559a3256d8f901930f571e0-0ee8c60dd263d7b0.webp](https://apollo-media.codio.com/media%2F1%2F61868b3b9559a3256d8f901930f571e0-0ee8c60dd263d7b0.webp)

As we find these pixels we can find the bounding box by finding the minimum and maximum row and column. A bounding box can be defined by the minimum row and column (i.e. 1,1) and the maximum row and column (i.e. 4,3). We will store this info using a `Location` struct (row/col) for the **upper-left** point of the bounding box and then two separate integers **height** and **width**.

The height and width should be the number of rows and columns that have pixels in the component/blob. In the picture above the height is **4** and width of **3**.

So notice that if you added the height to the upper-left (min) row you’d get 1 + 4 = 5 which is **“one beyond the last row”**. So just take care to realize upper-left row + height is not a row that has a pixel in the component. **The same is true of width.** We take this tactic since it allows writing loops using our common less-than `<` idiom.

*`// i will iterate through all rows that // have a black pixel in the component***for**(**int** i=ul.row; i < ul.row + height; i++)`

So the **upper-left** `Location` is just the **minimum** row of ANY pixel in the digit and the **minimum** column of any pixel in the digit. To find the **height** and **width**, we need the **maximum** row of ANY pixel in the digit and the **maximum** column of any pixel in the digit and then compute: height=rowmax−rowmin+1height=rowmax−rowmin+1 and width=colmax−colmin+1width=colmax−colmin+1

So for the image above we would store the upper-left location (i.e. 1,1) along with a height (4-1+1 = 4) and width (3-1+1 = 3).

## **Digit blobs**

To model a single digit, we have created a `DigitBlob` class. At minimum, it will store:

- the **raw pointer to the 2D image array** of pixels (not the `NumImg`).
- the **upper-left `Location`**, **height**, and **width** of the bounding box (and while the `DigitBlob` has a pointer to the whole image, it should only access the pixels in or surrounding the bounding box.)
- the `char` of the classified digit
- Data members for the feature analysis (some which we have declared and any others that you wish to add)

A `DigitBlob` will be constructed from the bounding box of a digit character that your `NumImg` class finds. The `NumImg` class can then store the `DigitBlob`s in a `vector<DigitBlob> blobs_` data member.

Here is an image of the overall organization of the classes and their relationship to each other:

![https://decimalrodeo-icebergsphere.codio.io/.guides/img/class-org.png](https://decimalrodeo-icebergsphere.codio.io/.guides/img/class-org.png)

The primary operation we will perform on a `DigitBlob` is to **classify** it and determine which digit `0`-`9` we believe it is. To make this classification, we need to run tests or extract features that can distinguish between each digit. In modern computer science, machine learning techniques such as neural networks would likely be used. However, to keep things simple and have you practice certain programming techniques, we will use more “brute-force” feature analysis and tests. We will describe a few to you but you are welcome to define others.

## **Classification Techniques / Tests**

On the spectrum of possible classification techniques is some kind of template matching where we take a known pixel/bitmap of a character and compare it. However, this does not account for noise in scanning/image generation, font differences, etc. and thus is usually a poor choice by itself. Better classification techniques will be be more tolerant of sizing, stretching, etc. However, to keep things simple we will make some assumptions:

- Characters will always be upright (non-rotated) and not unfairly stretched
- Characters may appear anywhere in the image but will always be at least 2 rows and 2 columns in from the boundaries of the image and neighboring bounding boxes

To help classify your characters, assume we perform a set of tests on the portion of the main image inside the bounding box region. Some useful tests may include:

- **Euler number**: This measure is a bit esoteric but is quite useful for our task. The definition is: EulerNumber=Components−HolesEulerNumber=Components−Holes where
    - `Components` = The number of unique connected components to the character (always 1 for our characters)
    - `Holes` = The number of (enclosed) areas in the character.Essentially, this gives us a way to detect . Since each digit forms 1 connected component, the Euler number is  and can easily help us partition digits into sets that have:
        
        **how many enclosed “holes” appear in the digit**
        
        1−Holes
        
        1−Holes
        
        - 2 holes (i.e. `8`),
        - 1 hole (i.e. `0`, `4`, `6`, and `9`) and
        - 0 holes (`1`, `2`, `3`, `5`, `7`). More on how to compute this below.
- **Aspect Ratio**: Height of the character divided by the width of the character
- **Vertical Centroid (1st moment)**: Intuitively measures the vertical **center of mass**:

∑h,wi,j=0,0i∗is_black(i,j)∑h,wi,j=0,0is_black(i,j)∑i,j=0,0h,wi∗is_black(i,j)∑i,j=0,0h,wis_black(i,j)

where is_black(i,j)=1,is_black(i,j)=1, if the pixel at row ii, column jj is black and 00, otherwise.

The summation just indicates to iterate over the entire height and width of the bounding box. Thus, the denominator is just the number of black pixels in the digit. The numerator helps to produce a **weighted** average for the row.

See examples in the accompanying video for more information.

By normalizing this to the height of the character (i.e. dividing the vertical center of mass by the height of the bounding box) we can arrive at a value between 0-1 as to where the vertical center of mass is: a value less 0.50.5 would mean an average in the upper half of the bounding box while anything above than 0.50.5 would indicate the average in the lower half of the bounding box.

- **Horizontal Centroid (1st moment)**: Intuitively measures the horizontal **center of mass** in the same way as the vertical centroid:

∑h,wi,j=0,0j∗is_black(i,j)∑h,wi,j=0,0is_black(i,j)∑i,j=0,0h,wj∗is_black(i,j)∑i,j=0,0h,wis_black(i,j)

where is_black(i,j)=1,is_black(i,j)=1, if the pixel at row ii, column jj is black and 00, otherwise. See examples in the accompanying video for more information. By normalizing this to the height of the character we can arrive at a value between 0-1 as to where the vertical center of mass is.

- **Vertical Symmetry**: A normalized measurement between 00 and 11 that indicates whether **pixels in the upper half are a mirror image to the lower half** (e.g. if a pixel in row 0, column c matches the one in row H-1, column c; if a pixel in row 1, column c matches a pixel in row H-2, column c, etc). 11 indicates a perfect match of the upper and lower half.
- **Horizontal Symmetry**: A measurement between 00 and 11 that indicates whether **pixels in the left half are a mirror image to the right half** (e.g. if a pixel in row r, column 0 matches the one in row r, column W-1; if a pixel in row r, column 1 matches a pixel in row r, column W-2). 11 indicates a perfect match of the left and right half.
- **Other Measures**: Other measures that might be of help could be what fraction of black pixels is in various parts of the bounding box (upper or lower quarter or third vs. the middle quarter or third, some portion of the corner regions, etc.) as well as the number and location of intersections of vertical or horizontal line(s) through the digit. These, and others you may think of, are at your discretion to implement.

### **Bit Quads to Compute Euler Number**

Identification of optical characters and images is often facilitated by computing important geometric and topological properties/features. Several of these features (including Euler number, perimeter, etc), can be calculated by looking at 2x2 windows of pixels using a technique referred to as Bit Quads.

A bit quad is a 2 X 2 array of pixels cells that can only be black or white (i.e. 2x2 windows of our image). There are 16 possible bit quads, and six distinct types considering rotational equivalence: BQ0,BQ1,BQ2,BQd,BQ3,BQ4BQ0,BQ1,BQ2,BQd,BQ3,BQ4. Their patterns are shown below.

![https://decimalrodeo-icebergsphere.codio.io/.guides/img/bit-quad-patterns.png](https://decimalrodeo-icebergsphere.codio.io/.guides/img/bit-quad-patterns.png)

Using these template patterns, we can proceed to **slide** a 2x2 window through image representing the digit and **count** the occurrences of the different bit quads.

![https://decimalrodeo-icebergsphere.codio.io/.guides/img/bit-quad-process.png](https://decimalrodeo-icebergsphere.codio.io/.guides/img/bit-quad-process.png)

It turns out the Euler number can be computed directly from these bit quad counts as:

Euler number=nBQ1−nBQ3−2∗nBQd4Euler number=nBQ1−nBQ3−2∗nBQd4

It might seem a bit mystical how that formula came about but with a little reasoning it can be deduced.

**Important Note**: When calculating bit quads you need to make sure you start on a **blank row and column and end on a blank row and column**. Thus rather than just starting on the upper-left row/column of the bounding box. which is the row and column where black pixels exist we need to iterate over the bounding box expanded back 1 step in each direction. In the images we give you we will guarantee that these rows and column are white. Thus, if your bounding box has an upper-left point at (sr,sc) and height and width of `h` and `w` respectively, you should count bit quads over the region using the following loops to iterate where `i` and `j` are the coordinates of the top-left pixel of the 2x2 window to examine:

**`for**(i=ul_.row-1; i < ul_.row + h_ ; i++){
  **for**(j=ul_.col-1; j < ul_.col + w_ ; j++){
    ...
  }
}`

## **Images => Digit Characters => Strings => BigInts**

While correctly recognizing the digit characters is the main part of your work, the last step is to practice operator overloading and implement arithmetic algorithms by updating your `BigInt` class from your previous assignment to support `operator+`, `operator-`, and `operator<`.

We have provided a `main()` application in `ocr.cpp` that needs only minor additions where we have placed comments. It should

- Read in the two `.bmp` images
- Create `NumImg` objects for each image
- Call the `findAndCreateDigitBlobs()` and `classify()` functions on each `NumImg`.
- Create `BigInt` objects from the string of classified characters from each `NumImg`
- Output the sum of those integers and the difference of the larger minus the smaller.

## **BigInt Features to Implement**

You can copy/paste your current `BigInt` implementation by viewing your previous assignment (**DO NOT MARK IT INCOMPLETE if it is currently COMPLETE**, but just open it). Select each file (`bigint.h` and `bigint.cpp`) and copy/paste its contents into the blank versions in the skeleton of this project.

### **Operator+**

Your current `add()` operation effectively implements `operator+=`. Write a new member function whose prototype would be:

`BigInt **operator**+(**const** BigInt& rhs);`

It should not modify the left-hand side calling object but create and return a BigInt (return-by-value) that has the sum.

### **Operator<**

Write a new member function whose prototype would be:

**`bool** **operator**<(**const** BigInt& rhs);`

It should return true if the magnitude of the left-hand side (calling object) is **less-than** the right-hand side `BigInt`.

Think about how to deal with `BigInt`s of different sizes, but **take care to ensure leading 0s are handled appropriately** and do not cause errors when comparing.

While there are a few ways to perform the computation, realize that the *more-significant* digits of the number (the left-most digits when printed or written) take priority when determining the comparison results.

### **Operator-**

Write a new member function whose prototype would be:

`BigInt **operator**-(**const** BigInt& rhs);`

It should return a `BigInt` that holds the difference of the calling (left-hand side) object minus the right-hand side object.

The GIF image below shows the subtraction process you learned in grade school. This is often called **Column Subtraction**. You would write the numbers one on top of each other, and subtract digits in the columns from right to left.

![https://decimalrodeo-icebergsphere.codio.io/.guides/img/subtract_new.gif](https://decimalrodeo-icebergsphere.codio.io/.guides/img/subtract_new.gif)

Sometimes when you are subtracting numbers, the top digit in a column is smaller than the bottom digit in that column. For this, you need to **“borrow”** from the next column. The borrowing process is shown in blue in the above GIF image. Please take a moment to study the algorithm and think about how you can write out the process programmatically.

**We will not require you to support negative numbers**. We provided the `<` operator so that you could always do **larger** - **smaller**. In other words, if we do **smaller** - **larger**, you are not responsible for what your code does.

**Note:** You can earn credit for these tasks independent of the character/digit recognition, as we will provide some separate tests for this code.