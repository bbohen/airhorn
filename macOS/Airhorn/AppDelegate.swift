//
//  AppDelegate.swift
//  Airhorn
//
//  Created by Brad Bohen on 8/10/19.
//  Copyright © 2019 Brad Bohen. All rights reserved.
//

import AVFoundation
import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate, AVAudioPlayerDelegate {

    var players: [URL: AVAudioPlayer] = [:]
    var duplicatePlayers: [AVAudioPlayer] = []
    
    let statusItem = NSStatusBar.system.statusItem(withLength:NSStatusItem.squareLength)

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        if let button = statusItem.button {
            button.image = NSImage(named:NSImage.Name("Trumpet"))
            button.action = #selector(self.clickHandler(sender:))
            button.sendAction(on: [.leftMouseUp, .rightMouseUp])
        }
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }
    
    @objc func clickHandler(sender: NSStatusBarButton) {
        let event = NSApp.currentEvent!
        
        if event.type == NSEvent.EventType.rightMouseUp {
            NSApplication.shared.terminate(self)
        } else {
            let path = Bundle.main.path(forResource: "airhorn", ofType: "m4a")!
            let soundFileNameURL = URL(fileURLWithPath: path)
            
            if let player = players[soundFileNameURL] { //player for sound has been found
                
                if !player.isPlaying { //player is not in use, so use that one
                    player.prepareToPlay()
                    player.play()
                } else { // player is in use, create a new, duplicate, player and use that instead
                    
                    do {
                        let duplicatePlayer = try AVAudioPlayer(contentsOf: soundFileNameURL)
                        
                        duplicatePlayer.delegate = self
                        //assign delegate for duplicatePlayer so delegate can remove the duplicate once it's stopped playing
                        
                        duplicatePlayers.append(duplicatePlayer)
                        //add duplicate to array so it doesn't get removed from memory before finishing
                        
                        duplicatePlayer.prepareToPlay()
                        duplicatePlayer.play()
                    } catch let error {
                        print(error.localizedDescription)
                    }
                }
            } else { //player has not been found, create a new player with the URL if possible
                
                do {
                    let player = try AVAudioPlayer(contentsOf: soundFileNameURL)
                    players[soundFileNameURL] = player
                    player.prepareToPlay()
                    player.play()
                } catch let error {
                    print(error.localizedDescription)
                }
            }
        }
    }
}

