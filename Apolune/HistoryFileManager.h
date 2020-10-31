/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

/*
 * Proposed save file format:
 *
 * 1..n files per system (one for the system, one for each planet)
 * Named history/player#/system#.txt / history/player#/system#-planet#.txt, etc..
 *
 * Example:
 * history/0/player.txt  // player profile (starship, inventory, etc)
 * history/0/catalog.txt // player-specific component catalog?
 * history/0/1.txt       // contains starbases and other info for system 1
 * history/0/1-1.txt     // contains any info for planet 1
 * history/0/1-1-1.txt   // contains any info for planet 1's first moon
 *
 * "Any info" is defined as "save-worthy" info of the following types:
 *  - player spawned buildings that have been placed
 *  - player named planets, radar notes for known locations
 *  - interior maps for any buildings that have been entered
 *  - container contents on items flagged save-me (a flag turned on when the item has been opened)
 *  - "empire" information
 */

class HistoryFileManager
{
public:
 HistoryFileManager(void) {
 }
 ~HistoryFileManager(void) {
 }
};

