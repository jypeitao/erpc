package com.dreamsmart.erpclib

class NativeLib {

    /**
     * A native method that is implemented by the 'erpclib' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'erpclib' library on application startup.
        init {
            System.loadLibrary("erpclib")
        }
    }
}