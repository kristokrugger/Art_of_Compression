#include "stats.h"
#include <iostream>
/* returns the sums of all pixel values across all color channels.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr)
{
	long sum = 0;

	switch(channel)
	{
		case 'r':
		{
			sum = sumRed[lr.second][lr.first];
			if(ul.first > 0)
			{
				if(ul.second > 0)
				{
					sum += sumRed[ul.second-1][ul.first-1];

				}
				sum -= sumRed[lr.second][ul.first-1];
			}
			if(ul.second > 0)
			{
				sum -= sumRed[ul.second-1][lr.first];
			}
		}
		break;
		case 'g':
		{
			sum = sumGreen[lr.second][lr.first];
			if(ul.first > 0)
			{
				if(ul.second > 0)
				{
					sum += sumGreen[ul.second-1][ul.first-1];

				}
				sum -= sumGreen[lr.second][ul.first-1];
			}
			if(ul.second > 0)
			{
				sum -= sumGreen[ul.second-1][lr.first];
			}

		}
		break;
		case 'b':
			sum = sumBlue[lr.second][lr.first];
			if(ul.first > 0)
			{
				if(ul.second > 0)
				{
					sum += sumBlue[ul.second-1][ul.first-1];

				}
				sum -= sumBlue[lr.second][ul.first-1];
			}
			if(ul.second > 0)
			{
				sum -= sumBlue[ul.second-1][lr.first];
			}
		break;
	}
	return sum;

}

/* returns the sums of squares of all pixel values across all color channels.
* useful in computing the score of a rectangle
* PA3 function
* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr)
{
	long sum = 0;

	switch(channel)
	{
		case 'r':
		{
			sum = sumsqRed[lr.second][lr.first];
			if(ul.first > 0)
			{
				if(ul.second > 0)
				{
					sum += sumsqRed[ul.second-1][ul.first-1];

				}
				sum -= sumsqRed[lr.second][ul.first-1];
			}
			if(ul.second > 0)
			{
				sum -= sumsqRed[ul.second-1][lr.first];
			}
		}
		break;
		case 'g':
		{
			sum = sumsqGreen[lr.second][lr.first];
			if(ul.first > 0)
			{
				if(ul.second > 0)
				{
					sum += sumsqGreen[ul.second-1][ul.first-1];

				}
				sum -= sumsqGreen[lr.second][ul.first-1];
			}
			if(ul.second > 0)
			{
				sum -= sumsqGreen[ul.second-1][lr.first];
			}

		}
		break;
		case 'b':
			sum = sumsqBlue[lr.second][lr.first];
			if(ul.first > 0)
			{
				if(ul.second > 0)
				{
					sum += sumsqBlue[ul.second-1][ul.first-1];

				}
				sum -= sumsqBlue[lr.second][ul.first-1];
			}
			if(ul.second > 0)
			{
				sum -= sumsqBlue[ul.second-1][lr.first];
			}
		break;
	}
	return sum;

}

// initialize the private vectors so that, for each color,  entry
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG & im)
{
	long sumR = 0;
	long sumG = 0;
	long sumB = 0;
  	long sumRs = 0;
	long sumGs = 0;
	long sumBs = 0;

	sumRed.resize(im.height(), vector<long>(im.width(), 0));
	sumGreen.resize(im.height(), vector<long>(im.width(), 0));
	sumBlue.resize(im.height(), vector<long>(im.width(), 0));
	sumsqRed.resize(im.height(), vector<long>(im.width(), 0));
	sumsqGreen.resize(im.height(), vector<long>(im.width(), 0));
	sumsqBlue.resize(im.height(), vector<long>(im.width(), 0));


	for(unsigned x = 0; x < im.height(); x++)
	{

		for(unsigned y = 0; y < im.width(); y++)
		{
			RGBAPixel * pixel = im.getPixel(y, x);

			sumR = pixel->r;
			sumG = pixel->g;
			sumB = pixel->b;

      		sumRs = pixel->r * pixel->r;
      		sumGs = pixel->g * pixel->g;
      		sumBs = pixel->b * pixel->b;

			if(x > 0)
			{
	        	sumR += sumRed[x-1][y];
	        	sumG += sumGreen[x-1][y];
	        	sumB += sumBlue[x-1][y];

		        sumRs += sumsqRed[x-1][y];
		        sumGs += sumsqGreen[x-1][y];
		        sumBs += sumsqBlue[x-1][y];
      		}

	      	if(y > 0)
	      	{
		        sumR += sumRed[x][y-1];
		        sumG += sumGreen[x][y-1];
		        sumB += sumBlue[x][y-1];

		        sumRs += sumsqRed[x][y-1];
		        sumGs += sumsqGreen[x][y-1];
		        sumBs += sumsqBlue[x][y-1];
	      	}

      		if(y > 0 && x > 0)
		  	{
		        sumR -= sumRed[x-1][y-1];
		        sumG -= sumGreen[x-1][y-1];
		        sumB -= sumBlue[x-1][y-1];

		        sumRs -= sumsqRed[x-1][y-1];
		        sumGs -= sumsqGreen[x-1][y-1];
		        sumBs -= sumsqBlue[x-1][y-1];
			}

			sumRed[x][y] = sumR ;
			sumsqRed[x][y] = sumRs;

			sumGreen[x][y] = sumG;
			sumsqGreen[x][y] = sumGs;

			sumBlue[x][y] = sumB;
			sumsqBlue[x][y] = sumBs;

		}
	}
}

// given a rectangle, compute its sum of squared deviations from
// mean, over all color channels. Will be used to make split when
// building tree.
/* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getScore(pair<int,int> ul, pair<int,int> lr)
{
	long R = rectArea(ul, lr);

	long sR = getSumSq('r', ul, lr) - ((getSum('r', ul, lr))*(getSum('r', ul, lr))) / R;
	long sG = getSumSq('g', ul, lr) - ((getSum('g', ul, lr))*(getSum('g', ul, lr))) / R;
	long sB = getSumSq('b', ul, lr) - ((getSum('b', ul, lr))*(getSum('b', ul, lr))) / R;

	long sS = sR + sG + sB;

	return sS;

}

// given a rectangle, return the average color value over the
// rectangle as a pixel.
/* Each color component of the pixel is the average value of that
* component over the rectangle.
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr)
{
	long R = rectArea(ul, lr);
	long r = getSum('r', ul, lr) / R;
	long g = getSum('g', ul, lr) / R;
	long b = getSum('b', ul, lr) / R;

	RGBAPixel avg(r, g, b);

	return avg;

}

// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::rectArea(pair<int,int> ul, pair<int,int> lr)
{
	long numPixels = 1;
	if(lr.first >= ul.first && lr.second >= ul.second)
		numPixels = (lr.first - ul.first + 1) * (lr.second - ul.second + 1);

	return numPixels;

}
