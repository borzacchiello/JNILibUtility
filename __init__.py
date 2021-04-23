from binaryninja import PluginCommand

from .find_jni_methods import FindJNIFunctionAnalysis


def locate_jni(bv):
    task = FindJNIFunctionAnalysis(bv)
    task.start()

PluginCommand.register(
    "Propagate JNI Types",
    "",
    locate_jni
)
