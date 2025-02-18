package com.dreamsmart.helloclient

import android.Manifest
import android.annotation.SuppressLint
import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.EditText
import android.widget.Switch
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.dreamsmart.erpc.spp.BluetoothSppManager

class MainActivity : AppCompatActivity() {

    companion object {
        init {
            System.loadLibrary("helloclient")
        }

        private val TAG = "helloclient"
    }

    private lateinit var mInitButton: Button
    private lateinit var mPrintButton: Button
    private lateinit var mStopButton: Button

    @SuppressLint("UseSwitchCompatOrMaterialCode")
    private lateinit var mSelectSwitch: Switch
    private lateinit var mSendText: EditText

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

    @SuppressLint("MissingPermission")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContentView(R.layout.activity_main)

        Log.d(TAG, "onCreate")
        if (!hasBluetoothPermission()) {
            Log.e(TAG, "no permission")
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
            Log.d(TAG, "init started?：${started}")
            if (started) {
                mInitButton.text = "init"
                destroy()
                started = true
            } else {
                var selectedDevice: BluetoothDevice? = null
                if (!useTcp) {
                    if (!hasBluetoothPermission()) {
                        Toast.makeText(
                            applicationContext,
                            "no bluetooth connect permission",
                            Toast.LENGTH_SHORT
                        ).show()
                        return@setOnClickListener
                    }

                    val bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
                    for (device: BluetoothDevice in bluetoothAdapter!!.bondedDevices) {
                        selectedDevice = device
                    }
                }

                if (selectedDevice == null) {
                    Toast.makeText(
                        applicationContext,
                        "没有找到要连接的蓝牙地址",
                        Toast.LENGTH_SHORT
                    ).show()
                    Log.d(TAG, "没有找到要连接的蓝牙地址")
                    return@setOnClickListener
                }

                BluetoothSppManager.setConnectDeviceAddress(selectedDevice.address)

                Log.d(TAG, "init use tcp：${useTcp}")
                var ret = init(useTcp, "localhost", 8899)
                if(ret) {
                    mInitButton.text = "destroy"
                    started = true
                    Log.d(TAG, "server connected")

                } else {
                    Toast.makeText(
                        applicationContext,
                        "远端服务先启动",
                        Toast.LENGTH_SHORT
                    ).show()
                }
            }
        }

        mSendText = findViewById(R.id.sendEditText)


        mPrintButton = findViewById(R.id.printButton)
        mPrintButton.setOnClickListener {
            Log.d(TAG, "print: "+mSendText.text.toString())
            Thread {
                // 执行耗时的 IO 操作
                val ret = callPrintText(mSendText.text.toString())
                Log.d(TAG, "print ret: $ret")
            }.start()

        }

        mStopButton = findViewById(R.id.stopButton)
        mStopButton.setOnClickListener {
            Log.d(TAG, "callStopServer")
            callStopServer()
        }


    }

    private external fun init(tcp: Boolean, address: String, port: Int): Boolean
    private external fun destroy()
    private external fun callPrintText(text: String): Boolean
    private external fun callStopServer()
}