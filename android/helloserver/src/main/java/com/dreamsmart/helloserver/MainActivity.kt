package com.dreamsmart.helloserver

import android.Manifest
import android.annotation.SuppressLint
import android.app.Activity
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.Switch
import android.widget.TextView
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.dreamsmart.erpc.spp.BluetoothSppManager

class MainActivity : AppCompatActivity() {
    companion object {
        init {
            System.loadLibrary("SppImpl")
            System.loadLibrary("helloserver")
        }

        private val TAG = "helloserver"
    }

    private lateinit var mInitButton: Button
    @SuppressLint("UseSwitchCompatOrMaterialCode")
    private lateinit var mSelectSwitch: Switch
    private lateinit var mReceiveTextView: TextView

    private var useTcp = false
    private var started = false

    private fun hasBluetoothPermission(): Boolean {
        val context = this

        return when {
            Build.VERSION.SDK_INT >= Build.VERSION_CODES.S -> {
                ActivityCompat.checkSelfPermission(
                    context,
                    Manifest.permission.BLUETOOTH_CONNECT
                ) == PackageManager.PERMISSION_GRANTED
            }

            else -> {
                ActivityCompat.checkSelfPermission(
                    context,
                    Manifest.permission.ACCESS_FINE_LOCATION
                ) == PackageManager.PERMISSION_GRANTED
            }
        }
    }

    private fun requestBluetoothPermission() {
        val context = this
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            ActivityCompat.requestPermissions(
                context as Activity,
                arrayOf(Manifest.permission.BLUETOOTH_CONNECT, Manifest.permission.BLUETOOTH_SCAN),
                1
            )
        } else
            ActivityCompat.requestPermissions(
                context as Activity,
                arrayOf(Manifest.permission.ACCESS_FINE_LOCATION),
                1
            )
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)


        Log.d(TAG,"onCreate")
        if(!hasBluetoothPermission()) {
            Log.e(TAG,"no permission")
            requestBluetoothPermission()

            return
        }

        mSelectSwitch = findViewById(R.id.switch1)
        mSelectSwitch.setOnCheckedChangeListener { _, isChecked ->
            if (isChecked) {
                useTcp = true
            } else {
                useTcp = false
            }
        }

        mInitButton = findViewById(R.id.initButton)
        mInitButton.setOnClickListener {
            if(started) {
                mInitButton.text = "init"
                stopServer()
                destroy()
                started = true
            }else {
                if(!useTcp) {
                    if(!hasBluetoothPermission()) {
                        Toast.makeText(applicationContext,"no bluetooth connect permission",Toast.LENGTH_SHORT).show()
                        return@setOnClickListener
                    }
                }
                mInitButton.text = "destroy"
                Log.d(TAG,"init use tcp：${useTcp}")
                init(useTcp)

                Log.d(TAG,"start server")
                starServer()
                started = true
            }
        }
        mReceiveTextView = findViewById(R.id.receiveTextView)
    }

    // call by native
    fun onPrintTextCall(txt: String) {
        runOnUiThread {
            mReceiveTextView.text = txt
        }
    }

    private external fun init(tcp: Boolean): Boolean
    private external fun destroy()
    private external fun starServer()
    private external fun stopServer()
}