/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: Tube_ctc.cpp
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

bool Tube::ctcFwd(Tube* derivative_tube)
{
  if(size() != derivative_tube->size())
    cout << "Warning ctcFwd(Tube* derivative_tube): tube of different size: "
         << derivative_tube->size() << "/" << size() << endl;

  bool contraction = false;

  for(int i = 1 ; i < size() ; i++) // from the past to the future
  {
    Interval y_old = (*this)[i];
    Interval y_new = y_old & ((*this)[i-1] + (*derivative_tube)[i-1] * derivative_tube->getSlice(i-1)->getT().diam());
    setY(y_new, i);
    contraction |= y_old.diam() > y_new.diam();
  }

  if(contraction)
    requestFutureTreeComputation();

  return contraction;
}

bool Tube::ctcBwd(Tube* derivative_tube)
{
  if(size() != derivative_tube->size())
    cout << "Warning ctcBwd(Tube* derivative_tube): tube of different size: "
         << derivative_tube->size() << "/" << size() << endl;

  bool contraction = false;

  for(int i = size() - 2 ; i >= 0 ; i--) // from the future to the past
  {
    Interval y_old = (*this)[i];
    Interval y_new = (*this)[i+1] - (*derivative_tube)[i+1] * derivative_tube->getSlice(i+1)->getT().diam();
    setY(y_new, i);
    contraction |= y_old.diam() > y_new.diam();
  }

  if(contraction)
    requestFutureTreeComputation();

  return contraction;
}

bool Tube::ctcFwdBwd(Tube* derivative_tube)
{
  bool contraction = false;
  contraction |= ctcFwd(derivative_tube);
  contraction |= ctcBwd(derivative_tube);
  return contraction;
}

bool Tube::ctcIn(Tube *derivative_tube, Interval& y, Interval& t)
{
  bool contraction;
  bool t_contraction = false;
  bool y_contraction = false;
  bool tube_contraction = false;

  // Trying to contract [t]
  {
    double t_diam = t.diam();

    // In forward
    int i = input2index(t.lb());
    while(!(*this)[i].intersects(y) && i < input2index(t.ub()))
    {
      i++;
      t = Interval(getT(i).lb(), t.ub()); // updating lower-bound (contraction)
    }

    // In backward
    i = input2index(t.ub());
    while(!(*this)[i].intersects(y) && i > input2index(t.lb()))
    {
      i--;
      t = Interval(t.lb(), getT(i).ub()); // updating upper-bound (contraction)
    }

    t_contraction = t.diam() < t_diam;
  }

  // Trying to contract [y]
  {
    double y_diam = y.diam();
    y &= (*this)[t];
    y_contraction = y.diam() < y_diam;
  }

  // Trying to contract this
  {
    Tube tube_temp1(*this);

    // Raw contraction without derivative information
    for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
    {
      Interval old_y = (*this)[i];
      Interval new_y = old_y & y;
      tube_temp1.setY(new_y, i);
    }

    // Taking derivative information into account
    Tube tube_temp2(tube_temp1);
    for(int i = input2index(t.lb()) ; i <= input2index(t.ub()) ; i++)
    {
      Tube tube_temp3(tube_temp1);
      Interval new_y = tube_temp3[i];

      // In forward
      for(int j = i ; j <= input2index(t.ub()) ; j++)
      {
        new_y += Interval((*derivative_tube)[j].lb() * derivative_tube->getT(j).diam(),
                          (*derivative_tube)[j].ub() * derivative_tube->getT(j).diam());
        tube_temp3.setY(new_y, j);
      }
      new_y = tube_temp1[i];

      // In backward
      for(int j = i ; j >= input2index(t.lb()) ; j--)
      {
        Interval deriv = ((*derivative_tube)[j]);
        new_y -= Interval(deriv.lb() * derivative_tube->getT(j).diam(),
                          deriv.ub() * derivative_tube->getT(j).diam());
        tube_temp3.setY(new_y | tube_temp3[j], j); // an union is made to keep forward's update
      }

      tube_temp2 |= tube_temp3;
    }

    double vol = volume();
    (*this) &= tube_temp2;
    tube_contraction = volume() < vol;
  }

  contraction = t_contraction | tube_contraction | y_contraction;

  if(contraction)
    requestFutureTreeComputation();

  return contraction;
}