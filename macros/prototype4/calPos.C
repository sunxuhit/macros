#include "TMath.h"

int calPos()
{
  double theta = 90-46.4;
  double angle = theta*TMath::Pi()/180.;
  double pos_x[11] = {120.0,122.0,124.0,126.0,128.0,190.0,200.0,210.0,220.0,230.0,240.0};
  for(int i_pos = 0; i_pos < 11; ++i_pos)
  {
    double vertex_z = pos_x[i_pos]*TMath::Cos(angle) + 40;
    cout << "pos_x = " << pos_x[i_pos] << ", pos_z = " << vertex_z << endl;
  }
}
