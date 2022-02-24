from contextlib import ContextDecorator
import time

class ExecTimer(ContextDecorator):
    def __init__(self,  fctName=''):
      self.fctName=fctName

    def __enter__(self):
        self.t0=time.time()
        return self

    def __exit__(self, *exc):
        print('[%s] exec time: %f'%(self.fctName, (time.time() - self.t0)))
        return False
