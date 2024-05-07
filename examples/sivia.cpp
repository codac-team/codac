#define W 1200
#define H 600

void SIVIA_sep(const Sep& cf, const IntervalVector& x0, double eps, const std::string& figname)
{
  vibes::newFigure(figname);
  vibes::axisLimits(x0[0].lb(),x0[0].ub(),x0[1].lb(),x0[1].ub());
  vibes::setFigureProperties(vibesParams("x",600, "y",100, "width",W, "height",H));

  clock_t t_start = clock();
  
  list<IntervalVector> l;
  l.push_back(x0);
  
  while(!l.empty())
  {
    IntervalVector top = l.front();l.pop_front();

    auto x_ = cf.separate(top);
    auto boundary = x_.in & x_.out;

    for(const auto& bi : top.diff(x_.in))
      vibes::drawBox(bi[0].lb(),bi[0].ub(),bi[1].lb(),bi[1].ub(),"#9C9C9C[green]");

    for(const auto& bi : top.diff(x_.out))
      vibes::drawBox(bi[0].lb(),bi[0].ub(),bi[1].lb(),bi[1].ub(),"#9C9C9C[cyan]");

    if(!boundary.is_empty())
    {
      if(boundary.max_diam() < eps)
        vibes::drawBox(boundary[0].lb(),boundary[0].ub(),boundary[1].lb(),boundary[1].ub(),"#9C9C9C[yellow]");
      else
      {
        auto p = boundary.bisect_largest();
        l.push_back(p.first); l.push_back(p.second);
      }
    }
  }
  
  printf("Computation time: %.4fs\n", (double)(clock() - t_start)/CLOCKS_PER_SEC);
}

template<typename C>
void SIVIA(const C& cf, const IntervalVector& x0, double eps, const std::string& figname, const std::string& c_maybe = "yellow", bool new_fig = true)
{
  if(new_fig)
  {
    vibes::newFigure(figname);
    vibes::axisLimits(x0[0].lb(),x0[0].ub(),x0[1].lb(),x0[1].ub());
    vibes::setFigureProperties(vibesParams("x",600, "y",100, "width",W, "height",H));
  }

  clock_t t_start = clock();

  if(new_fig)
    vibes::drawBox(x0[0].lb(),x0[0].ub(),x0[1].lb(),x0[1].ub(),"#9C9C9C[cyan]");
  list<IntervalVector> l;
  l.push_back(x0);
  int n = 0;

  while(!l.empty())
  {
    IntervalVector top = l.front();
    l.pop_front();

    auto x_ = top;
    cf.contract(top);

    if(!top.is_empty())
    {
      if(top.max_diam() < eps)
      {
        n++;
        vibes::drawBox(top[0].lb(),top[0].ub(),top[1].lb(),top[1].ub(),"#9C9C9C[" + c_maybe + "]");
      }

      else
      {
        auto p = top.bisect_largest();
        l.push_back(p.first); l.push_back(p.second);
      }
    }
  }
  
  printf("Computation time: %.4fs, %d boxes\n", (double)(clock() - t_start)/CLOCKS_PER_SEC, n);

  if(new_fig)
  {
    vibes::axisLimits(x0[0].lb(),x0[0].ub(),x0[1].lb(),x0[1].ub());
    vibes::setFigureProperties(vibesParams("x",600, "y",100, "width",W, "height",H)); 
  }
}