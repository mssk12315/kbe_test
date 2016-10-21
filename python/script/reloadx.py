import sys    
import os    
class Reloader:    
    SUFFIX = '.pyc'    
    
    def __init__(self):    
        self.mtimes = {}    
    
    def __call__(self):    
        import pdb    
        pdb.set_trace()    
        for mod in sys.modules.values():    
            self.check(mod)    
    
    def check(self, mod):    
        if not (mod and hasattr(mod, '__file__') and mod.__file__):    
            return     
        try:    
            mtime = os.stat(mod.__file__).st_mtime    
        except (OSError, IOError):    
            return    
        if mod.__file__.endswith(self.__class__.SUFFIX) and os.path.exist(mod.__file__[:-1]):    
            mtime = max(os.stat(mod.__file__[:-1].st_mtime), mtime)    
        if mod not in self.mtimes:    
            self.mtimes[mod] = mtime    
        elif self.mtimes[mod] < mtime:    
            try:    
                reload(mod)    
                self.mtimes[mod] = mtime    
            except ImportError:    
                pass    
    
reloader = Reloader()    
reloader()   