//
//  AppDelegate.swift
//  Airhorn
//
//  Created by Brad Bohen on 8/10/19.
//  Copyright © 2019 Brad Bohen. All rights reserved.
//

import AVFoundation
import Cocoa

class AppDelegate: NSObject, NSApplicationDelegate, AVAudioPlayerDelegate {
    
    var players: [URL: AVAudioPlayer] = [:]
    var duplicatePlayers: [AVAudioPlayer] = []
    var statusItem: NSStatusItem!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Create status bar item
        statusItem = NSStatusBar.system.statusItem(withLength: NSStatusItem.squareLength)
        
        guard let button = statusItem.button else {
            NSLog("❌ ERROR: Could not create status bar button")
            return
        }
        
        // Set up the icon
        if let trumpetImage = NSImage(named: "Trumpet") {
            trumpetImage.isTemplate = true
            button.image = trumpetImage
        } else {
            // Fallback to emoji if image doesn't load
            button.title = "📯"
        }
        
        button.action = #selector(self.clickHandler(sender:))
        button.sendAction(on: [.leftMouseUp, .rightMouseUp])
    }
    
    @objc func clickHandler(sender: NSStatusBarButton) {
        let event = NSApp.currentEvent!
        
        if event.type == NSEvent.EventType.rightMouseUp {
            // Right click = quit
            NSApplication.shared.terminate(self)
        } else {
            // Left click = play airhorn sound
            guard let path = Bundle.main.path(forResource: "airhorn", ofType: "m4a") else {
                NSLog("❌ Could not find airhorn.m4a")
                return
            }
            
            let soundFileNameURL = URL(fileURLWithPath: path)
            
            if let player = players[soundFileNameURL] {
                // Player for sound has been found
                if !player.isPlaying {
                    // Player is not in use, so use that one
                    player.prepareToPlay()
                    player.play()
                } else {
                    // Player is in use, create a new duplicate player
                    do {
                        let duplicatePlayer = try AVAudioPlayer(contentsOf: soundFileNameURL)
                        duplicatePlayer.delegate = self
                        duplicatePlayers.append(duplicatePlayer)
                        duplicatePlayer.prepareToPlay()
                        duplicatePlayer.play()
                    } catch let error {
                        NSLog("❌ Error creating duplicate player: \(error.localizedDescription)")
                    }
                }
            } else {
                // Player has not been found, create a new player
                do {
                    let player = try AVAudioPlayer(contentsOf: soundFileNameURL)
                    players[soundFileNameURL] = player
                    player.prepareToPlay()
                    player.play()
                } catch let error {
                    NSLog("❌ Error creating player: \(error.localizedDescription)")
                }
            }
        }
    }
    
    func audioPlayerDidFinishPlaying(_ player: AVAudioPlayer, successfully flag: Bool) {
        // Remove duplicate players after they finish playing
        duplicatePlayers.removeAll { $0 == player }
    }
}

