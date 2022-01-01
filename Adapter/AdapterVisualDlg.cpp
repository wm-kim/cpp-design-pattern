
// AdapterVisualDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdapterVisual.h"
#include "AdapterVisualDlg.h"
#include "afxdialogex.h"

#include <memory>
#include <map>
using namespace std;

vector<shared_ptr<VectorObject>> vectorObjects {
  make_shared<VectorRectangle>(10,10,100,100),
  make_shared<VectorRectangle>(30,30,60,60)
};

// construct bunch of points from line
struct LineToPointAdapter
{
  typedef vector<Point> Points;

  LineToPointAdapter(Line& line)
  {
    static int count = 0;
    TRACE("%d: Generating points for line (no caching)\n", count++);
    
    // no interpolation
    int left = min(line.start.x, line.end.x);
    int right = max(line.start.x, line.end.x);
    int top = min(line.start.y, line.end.y);
    int bottom = max(line.start.y, line.end.y);
    int dx = right - left;
    int dy = line.end.y - line.start.y;

    // only vertical or horizontal lines
    if (dx == 0)
    {
      // vertical
      for (int y = top; y <= bottom; ++y)
      {
        points.emplace_back(Point{ left,y });
      }
    }
    else if (dy == 0)
    {
      for (int x = left; x <= right; ++x)
      {
        points.emplace_back(Point{ x, top });
      }
    }
  }

  virtual Points::iterator begin() { return points.begin(); }
  virtual Points::iterator end() { return points.end(); }
private:
  Points points;
};

// cache about the information of the hash value of different lines
struct LineToPointCachingAdapter
{
  typedef vector<Point> Points;

  LineToPointCachingAdapter(Line& line)
  {
    // first check the hash
    boost::hash<Line> hash;
    line_hash = hash(line);
    if (cache.find(line_hash) != cache.end()) return;

    static int count = 0;
    TRACE("%d: Generating points for line (with caching)\n", count++);

    // no interpolation
    Points points;

    int left = min(line.start.x, line.end.x);
    int right = max(line.start.x, line.end.x);
    int top = min(line.start.y, line.end.y);
    int bottom = max(line.start.y, line.end.y);
    int dx = right - left;
    int dy = line.end.y - line.start.y;

    // only vertical or horizontal lines
    if (dx == 0)
    {
      // vertical
      for (int y = top; y <= bottom; ++y)
      {
        points.emplace_back(Point{ left,y });
      }
    }
    else if (dy == 0)
    {
      for (int x = left; x <= right; ++x)
      {
        points.emplace_back(Point{ x, top });
      }
    }

    cache[line_hash] = points;
  }

  virtual Points::iterator begin() { return cache[line_hash].begin(); }
  virtual Points::iterator end() { return cache[line_hash].end(); }
private:
  // cache storage
  size_t line_hash; // last hash generated 
  static map<size_t, Points> cache;
};

/////////////////////////////////

void CAdapterVisualDlg::OnPaint()
{
    CPaintDC dc(this); // device context for painting

    if (IsIconic())
    {
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        // Draw the points :)
        for (auto& o : vectorObjects)
        {
            for (auto& l : *o)
            {
                LineToPointCachingAdapter lpo{ l };
                DrawPoints(dc, lpo.begin(), lpo.end());
            }
        }

        CDialogEx::OnPaint();
    }

}