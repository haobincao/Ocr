#include "digitblob.h"
#include <iostream>
// Add more includes if necessary
#include <vector>

using namespace std;

// TO DO - Complete this function
bool Location::operator<(const Location& other) const
{
    if(this->col < other.col)  {
        return true;
    }
    else if(this->col > other.col)  {
        return false;
    }
    if(this->row < other.row)  {
        return true;
    }
    return false;
}

// TO DO - Complete this function
DigitBlob::DigitBlob()
{
    img_ = NULL;
    digit_ = '!'; // dummy value
    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // ul_'s Location default constructor already initializes it to -1,-1

    // Initilaize h_ and w_ and any other data members
    h_ = 0;
    w_ = 0;
    ul_ = Location(-1, -1);

}

// TO DO - Complete this function
DigitBlob::DigitBlob(uint8_t** img, Location upperleft, int height, int width)
{
    img_ = img;
    digit_ = '!'; // dummy value

    bq0_ = bq1_ = bq2_ = bq3_ = bq4_ = bqd_ = 0;
    euler_ = -2;

    // Initilaize ul_, h_ and w_ and any other data members
    h_ = height;
    w_ = width;
    ul_ = upperleft;

}

// TO DO - Complete this function if necessary
DigitBlob::~DigitBlob()
{
    // Add code if it is necessary

}

// TO DO - Complete this function
void DigitBlob::classify()
{
    calc_bit_quads();
    calc_euler_number();
    calc_horS();
    calc_verS();
    // Call helper functions to calculate features
    calc_horC();
    calc_verC();
    calc_aspect_ratio();
    // TO DO: use the results of helper functions to calculate features
    //    We suggest starting with the Euler number but you are free to
    //    change our structure
    if(euler_ == -1)  {
        digit_ = '8';
    }
    else if(euler_ == 0)  {
      if(horS >= 0.95 || verS >= 0.95)  {
        digit_ = '0';
      }
      else if(verC > horC)  {
        digit_ = '6';
      }
      else if(verS > horS)  {
        digit_ = '4';
      }
      else  {
        digit_ = '9';
      }
    }
    else  {
      if(horS >= 0.85 || max(horS, verS) - min(verS, horS) < 0.005)  {
        digit_ = '1';
      }
      else if(horS < verS)  {
        if(verC > horC)   {
          digit_ = '2';
        }
        else  {
          digit_ = '3';
        }
      }
      else if(verC < 0.4)  {
        digit_ = '7';
      }
      else if(verC < 0.515 && horC < 0.515)  {
        digit_ = '5';
      }
      else if(horC > 0.5 && verC > 0.5)  {
        digit_ = '2';
      }
      else  {
        digit_ = '7';
      }
    }


}

// Complete - Do not alter
char DigitBlob::getClassification() const
{
    return digit_;
}

// TO DO - Complete this function
void DigitBlob::printClassificationResults() const
{
    cout << "Digit blob at " << ul_.row << "," << ul_.col << " h=" << h_ << " w=" << w_ << endl;
    cout << "Bit quads: 1, 2, D, 3, 4:";
    cout << " " << bq1_ << " " << bq2_ << " " << bqd_;
    cout << " " << bq3_ << " " << bq4_ << endl;
    cout << verC << " " << horC << endl;
    cout << aspect << endl;
    cout << verS << " " << horS << endl;
    cout << "Euler number is " << euler_ << endl;
    // TO DO: Add any other couts to print classification test data





    cout << "****Classified as: " << digit_ << "\n\n" << endl;

}

// Complete - Do not alter
const Location& DigitBlob::getUpperLeft() const
{
    return ul_;
}

// Complete - Do not alter
int DigitBlob::getHeight() const
{
    return h_;
}

// Complete - Do not alter
int DigitBlob::getWidth() const
{
    return w_;
}

// Complete - Do not alter
bool DigitBlob::operator<(const DigitBlob& other)
{
    // Use Location's operator< for DigitBlob operator<
    return ul_ < other.ul_;
}

// Complete - Do not alter
void DigitBlob::calc_euler_number()
{
    euler_ = (bq1_ - bq3_ - 2*bqd_) / 4;
}

// TO DO - Complete this function to set bq1_, bq2_, etc.
void DigitBlob::calc_bit_quads()
{
  for(int i=ul_.row-1; i < ul_.row + h_ ; i++){
    for(int j=ul_.col-1; j < ul_.col + w_ ; j++){
      int count = 0;
      if(img_[i][j] == 0)  {
        count++;
      }
      if(img_[i+1][j] == 0)  {
        count++;
      }
      if(img_[i][j+1] == 0)  {
        count++;
      }
      if(img_[i+1][j+1] == 0)  {
        count++;
      }
      if(count == 0)  {
        bq0_++;
      }
      else if(count == 1)  {
        bq1_++;
      }
      else if(count == 3)  {
        bq3_++;
      }
      else if(count == 4)  {
        bq4_++;
      }
      else if(img_[i+1][j+1] == 0 && img_[i][j] == 0)  {
        bqd_++;
      }
      else if(img_[i][j+1] == 0 && img_[i+1][j] == 0)  {
        bqd_++;
      }
      else  {
        bq2_++;
      }
    }
  }


}

// Add more private helper function implementations below
void DigitBlob::calc_verC()  {
  int count = 0;
  double sum = 0;
  for(int i = 0; i < h_; i++)  {
    for(int j = 0; j < w_; j++)  {
      if(img_[i+ul_.row][j+ul_.col] == 0)  {
        count++;
        sum += i;
      }
    }
  }
  verC = sum/count;
  verC /= (h_-1);
}

void DigitBlob::calc_horC()  {
  int count = 0;
  double sum = 0;
  for(int i = 0; i < h_; i++)  {
    for(int j = 0; j < w_; j++)  {
      if(img_[i+ul_.row][j+ul_.col] == 0)  {
        count++;
        sum += j;
      }
    }
  }
  horC = sum/count;
  horC /= (w_-1);
}

void DigitBlob::calc_aspect_ratio()  {
  aspect = (double)h_/w_;
}

void DigitBlob::calc_verS()  {
  double count = 0;
  double same = 0;
  for(int j = ul_.col; j < ul_.col+w_; j++)  {
    for(int i = 0; i <= h_/2; i++)  {
      count++;
      if(img_[i+ul_.row][j] == img_[ul_.row+h_-i-1][j])  {
        same++;
      }
    }
  }
  verS = same/count;
}

void DigitBlob::calc_horS()  {
  double count = 0;
  double same = 0;
  for(int j = ul_.row; j < ul_.row+h_; j++)  {
    for(int i = 0; i <= w_/2; i++)  {
      count += 1;
      if(img_[j][i+ul_.col] == img_[j][ul_.col+w_-i-1])  {
        same += 1;
      }
    }
  }
  horS = same/count;
}