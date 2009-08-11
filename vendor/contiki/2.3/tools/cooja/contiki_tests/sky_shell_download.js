TIMEOUT(150000); /* print last msg at timeout */

log.log("Random seed: " + mote.getSimulation().getRandomSeed() + "\n");

/* Wait for nodes to boot */
mote1 = null;
mote2 = null;
while (mote1 == null || mote2 == null) {
  if (id == 1) {
    mote1 = mote;
  }
  if (id == 2) {
    mote2 = mote;
  }
  YIELD();
}
GENERATE_MSG(2000, "continue");
YIELD_THEN_WAIT_UNTIL(msg.equals("continue"));

/* Generate initial file */
fileID = 1;
command = "echo *mote1data" + fileID + " | write file" + fileID + ".txt\n";
log.log("mote1> " + command);
write(mote1, command);
YIELD_THEN_WAIT_UNTIL(id == 1 && msg.contains("Contiki>"));

/* Download and append files */
while (fileID < 20) {
  /* Mote 1 -> Mote 2 */
  srcFile = "file" + fileID + ".txt";
  fileID++;
  dstFile = "file" + fileID + ".txt";
  command = "download 1.0 " + srcFile + " | write " + dstFile + " | null\n";
  log.log("mote2> " + command);
  write(mote2, command);
  YIELD_THEN_WAIT_UNTIL(id == 2 && msg.contains("Contiki>"));
  command = "echo *mote2data" + fileID + " | append " + dstFile + " | null\n";
  log.log("mote2> " + command);
  write(mote2, command);
  YIELD_THEN_WAIT_UNTIL(id == 2 && msg.contains("Contiki>"));

  /* Mote 2 -> Mote 1 */
  srcFile = "file" + fileID + ".txt";
  fileID++;
  dstFile = "file" + fileID + ".txt";
  command = "download 2.0 " + srcFile + " | write " + dstFile + " | null\n";
  log.log("mote1> " + command);
  write(mote1, command);
  YIELD_THEN_WAIT_UNTIL(id == 1 && msg.contains("Contiki>"));
  command = "echo *mote1data" + fileID + " | append " + dstFile + " | null\n";
  log.log("mote1> " + command);
  write(mote1, command);
  YIELD_THEN_WAIT_UNTIL(id == 1 && msg.contains("Contiki>"));
}

/* List files, verify contents */
/* XXX Beware of strange line breaks! (generated by shell's read command) */
log.log("Locating file21.txt on mote 1\n");
write(mote1, "ls\n");
YIELD_THEN_WAIT_UNTIL(id == 1 && msg.contains("file21.txt"));
if (!msg.contains("264 ")) {
  log.log("Bad file size, should be 264 bytes: " + msg + "\n");
  log.testFailed();
}
YIELD_THEN_WAIT_UNTIL(id == 1 && msg.contains("Contiki>"));
log.log("Verifying file21.txt contents on mote 1\n");
write(mote1, "read file21.txt\n");
YIELD_THEN_WAIT_UNTIL(msg.contains("mote1data1"));
YIELD_THEN_WAIT_UNTIL(msg.contains("mote2data2"));
// ..
YIELD_THEN_WAIT_UNTIL(msg.contains("mote2data16") || msg.contains("mote2data18"));
YIELD_THEN_WAIT_UNTIL(msg.contains("mote1data19") || msg.contains("mote1data21"));

log.log("Test finished at time: " + mote.getSimulation().getSimulationTime() + "\n");

log.testOK(); /* Report test success and quit */
