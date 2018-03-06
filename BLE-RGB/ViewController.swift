//
//  ViewController.swift
//  BLE-RGB
//
//  Created by Yuki.Ma on 2017/12/10.
//  Copyright © 2017年 Yuki.Ma. All rights reserved.
//

import UIKit
extension UIImage{
    
    /**
     获取图片中的像素颜色值
     
     - parameter pos: 图片中的位置
     
     - returns: 颜色值
     */
    func getPixelColor(pos:CGPoint)->(alpha: CGFloat, red: CGFloat, green: CGFloat,blue:CGFloat){
        let pixelData=CGDataProvider(data: self.cgImage! as! CFData)//CGDataProviderCopyData(CGImageGetDataProvider(self.CGImage!))
        let data:UnsafePointer<UInt8> = CFDataGetBytePtr(pixelData as! CFData)
        let pixelInfo: Int = ((Int(self.size.width) * Int(pos.y)) + Int(pos.x)) * 4
        
        let r = CGFloat(data[pixelInfo]) / CGFloat(255.0)
        let g = CGFloat(data[pixelInfo+1]) / CGFloat(255.0)
        let b = CGFloat(data[pixelInfo+2]) / CGFloat(255.0)
        let a = CGFloat(data[pixelInfo+3]) / CGFloat(255.0)
        
        return (a,r,g,b)
    }
    
}


class ViewController: UIViewController {
    

    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view, typically from a nib.
        let rect = CGRect(x: 20, y: 60, width: 240, height: 240)
        let gradientView = UIView(frame: rect)
        
        let gradientLayer = CAGradientLayer()
        gradientLayer.frame = gradientView.frame
        
        let fromColor = UIColor.yellow.cgColor
        let midColor = UIColor.blue.cgColor
        let toColor = UIColor.red.cgColor
        
        gradientLayer.colors = [fromColor,midColor,toColor]
        gradientLayer.startPoint = CGPoint(x: 0, y: 0)
        gradientLayer.endPoint = CGPoint(x: 1, y: 1)
        gradientLayer.locations = [0,0.3,1]
        
        gradientView.layer.addSublayer(gradientLayer)
        self.view.addSubview(gradientView)
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

