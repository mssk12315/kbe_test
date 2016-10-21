# -*- coding: utf-8 -*-

# 热更新
# 当修改配置后,只需reload指定模块就可以在不重启服务的情况下读取修改的配置

import time
import sys, os

def auto_reload():
    mods = ["my_config"] # the need reload modules

    for mod in mods:
        try:
            module = sys.modules[mod]
        except:
            continue

        filename = module.__file__
        # .pyc修改时间不会变,又不能删除.pyc,所以就用.py的修改时间,如果有更好的办法就谢谢了.
        if filename.endswith(".pyc"):
            filename = filename.replace(".pyc", ".py")
        mod_time = os.path.getmtime(filename)
        if not "loadtime" in module.__dict__:
            module.loadtime = 0 # first load's time  1*

        try:
            if mod_time > module.loadtime:
                #reload(my_config) #for python 2.x
                import imp
                imp.reload(my_config)
                
        except:
            print("reload except")
            pass

        module.loadtime = mod_time # 2*


if __name__ == "__main__":
    import time
    import my_config
    tmp = None
    """模拟服务
    """
    while True:
        auto_reload()
        if my_config.ip != tmp:
            print( my_config.ip)
        tmp = my_config.ip
        time.sleep(2)
