import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.URL;

import javax.sound.sampled.AudioFormat;
import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.UnsupportedAudioFileException;

import edu.cmu.sphinx.frontend.util.StreamDataSource;
import edu.cmu.sphinx.recognizer.Recognizer;
import edu.cmu.sphinx.result.Result;
import edu.cmu.sphinx.util.props.ConfigurationManager;
import edu.cmu.sphinx.util.props.PropertyException;

public class Sphinx4TcpServer {
	
	private ServerSocket myServerSocket = null;
	private Socket mySocket = null;
	
	private static final int DEFAULT_PORT_NUMBER = 7000;
	private static final String DEFAULT_CONFIG_FILE = "config.xml";
	private static final String DEFAULT_WAV_FILE = "10001-90210-01803.wav"; // 12345.wav, 10001-90210-01803.wav
	
	private int portNumber = DEFAULT_PORT_NUMBER;
	private URL configURL = null;
	private URL audioFileURL = null;

	private boolean initialized = false;
	private boolean wavTest = false;
	
	public static void main(String[] args) {
		Sphinx4TcpServer server = new Sphinx4TcpServer();
		server.run(args);
    }
	
	public void run(String[] args)
	{
		if(!parseArguments(args)) {
			showUsage();
			return;
		}
		
		if(this.wavTest) {
			wavFileTest();
		}
		else {
			tcpStreamTest();
		}
	}
	
	private boolean parseArguments(String[] args)
	{
		this.configURL = Sphinx4TcpServer.class.getResource(DEFAULT_CONFIG_FILE);
		this.audioFileURL = Sphinx4TcpServer.class.getResource(DEFAULT_WAV_FILE);
		
		for(int i=0; i<args.length; i++) {
			if(args[i].compareTo("-help") == 0 || 
			   args[i].compareTo("--help") == 0 || 
			   args[i].compareTo("?") == 0) 
			{
				return false;
			}
			else if(args[i].compareTo("-p") == 0) {
				if(i+1 >= args.length) {
					System.err.println("Port required.");
					return false;
				}
				try {
					portNumber = Integer.parseInt(args[i+1]);
				}
				catch(NumberFormatException e) {
					System.err.println("Port " + args[i+1] + " is not valid");
					return false;
				}
			}
			else  if(args[i].compareTo("-config") == 0) {
				if(i+1 >= args.length) {
					System.err.println("Config file required.");
					return false;
				}
				URL tmpConfigURL = Sphinx4TcpServer.class.getResource(args[i+1]);
				if(tmpConfigURL == null) {
					System.err.println("The file " + args[i+1] + " is not a valid configuration file.");
					return false;
				}
				else {
					this.configURL = tmpConfigURL;
				}
				i++;
			}
			else if(args[i].compareTo("-wav") == 0) {
				this.wavTest = true;
				if(i+1 < args.length && args[i+1].charAt(0) != '-') {
					URL tmpAudioFileURL = Sphinx4TcpServer.class.getResource(args[i+1]);
					if(tmpAudioFileURL == null) {
						System.err.println("The file " + args[i+1] + " is not a valid wav.");
						return false;
					}
					else {
						this.audioFileURL = tmpAudioFileURL;
					}
					i++;
				}
			}
		}
		
		if(this.configURL == null) {
			System.err.println("The default Sphinx4 configuration file \"" + DEFAULT_CONFIG_FILE + "\" not found.");
			return false;
		}
		
		 if(this.wavTest && audioFileURL==null) {
         	System.err.println("wav file not found.");
         	return false;
         }
		
		return true;
	}
	
	private void showUsage()
	{
		System.out.println("Usage : java Sphinx4TcpServer [options]");
		System.out.println("Default:");
		System.out.println("     Port :                      7000");
		System.out.println("     Sphinx configuration file : config.xml");
		System.out.println("     If using wav :              10001-90210-01803.wav.");
		System.out.println("Options:");
		System.out.println("    -p #####                     Specify a port.");
		System.out.println("    -config fileName             Specify a sphinx4 configuration file.");
		System.out.println("    -wav [fileName]              Use a wav for the input stream,");
		System.out.println("                                    provided for convenience. The file");
		System.out.println("                                    name must not begins with a '-'.");
	}
	
	private boolean initTcp()
	{
		if(!initialized)
		{
			try {
				myServerSocket = new ServerSocket(portNumber);
				initialized = true;
			}
			catch(IOException e) {
				System.err.println(e);
			}	
		}
		return initialized;
	}
	
	private void deinitTcp()
	{
		try {
			if(mySocket != null) {
				mySocket.close();
				mySocket = null;
			}
			if(myServerSocket != null) {
				myServerSocket.close();
				myServerSocket = null;
			}
		}
		catch (IOException e) {
			System.err.println(e);
		}

	}
	
	private void initConnection(StreamDataSource reader)
	{
		try {
			if(!initialized) {
				System.err.println("init must be called before initConnection");
				return;
			}
			if(mySocket != null) {
				mySocket.close(); // Make sure that previous connection is terminated
			}
			
			System.err.println("Listening on port " + portNumber);
			mySocket = myServerSocket.accept();
			
			//cant_takeit.wav 8 bits
		    /*int rate  = 11025;
		    boolean signed = false;
		    int sampleSize = 8;*/
	
		    // 12345.wav 16 bits
		    /*int rate  = 16000;
		    boolean signed = true;
		    int sampleSize = 16;*/
		    int rate  = reader.PROP_SAMPLE_RATE_DEFAULT;//16000;
		    boolean signed = reader.PROP_SIGNED_DATA_DEFAULT;//true;
		    boolean bigEndian = reader.PROP_BIG_ENDIAN_DATA_DEFAULT;//true;
		    int sampleSize = reader.PROP_BITS_PER_SAMPLE_DEFAULT;//16;
		    	    
		    AudioFormat audioFormat = new AudioFormat(rate, sampleSize, 1, signed, bigEndian); 
		    
		    AudioInputStream audioInputStream = new AudioInputStream(mySocket.getInputStream(), audioFormat, AudioSystem.NOT_SPECIFIED);
		    
		    /* set the stream data source to read from the audio stream */
	        reader.setInputStream(audioInputStream, "tcp-stream");
		}
		catch(IOException e) {
			System.err.println(e);
		}
	}
		
	public void tcpStreamTest()
	{
		if(!initTcp()) {
			System.err.println("Initialization failed...");
			return;
		}
		try {          
            ConfigurationManager cm = new ConfigurationManager(this.configURL);

            System.err.println("Loading Recognizer...\n");
            Recognizer recognizer = (Recognizer) cm.lookup("recognizer");

            /* allocate the resource necessary for the recognizer */
            recognizer.allocate();
            
            // This listener will wait for a result to be generated by Sphinx4.
    	    // The result can be a partial result (recognition not finished)
    	    // or a final result (recognizer done with the utterance).
    	    // To make it fire more often, make "featureBlockSize" smaller
    	    // in the config file (for more info look at cmu's docs)
    	    /*recognizer.addResultListener(new ResultListener() {
    		    String resString = "";
    		    StringTokenizer myTok = null;
    		    int numToks = 0;

    		    public void newResult(Result result) {
	    		    resString = result.getBestResultNoFiller();
	    		    //if we have a partial result
	    		    if (!result.isFinal()) {
	    		    System.out.println(": Sphinx4 got partial result: " + resString);
	    		    //System.out.println(result.getBestToken().getWordUnitPath());
	    		    }
	    		    else {
	    		    	System.out.println(": Sphinx4 got final result: " + resString);
	    		    }
    		    }
    		    });*/

		    StreamDataSource reader
	                = (StreamDataSource) cm.lookup("streamDataSource");
		    
            
		    initConnection(reader);
		    
		    System.err.println("Entering loop...");
		    while(true) {
	            /* decode the audio file */
	            Result result = recognizer.recognize();
	            
	            /* print out the results */
	            if (result != null) {
	                System.out.println("RESULT: " + 
	                                   result.getBestFinalResultNoFiller() + "\n");
	            } else {
	                System.out.println("RESULT: null\n");
	                initConnection(reader);
	            }
		    }
        } catch (IOException e) {
            System.err.println("Problem with socket: " + e);
            e.printStackTrace();
        } catch (PropertyException e) {
            System.err.println("Problem configuring: " + e);
            e.printStackTrace();
        } catch (InstantiationException e) {
            System.err.println("Problem creating: " + e);
            e.printStackTrace();
        } catch (Exception e) {
            System.err.println("Unknown exception: " + e);
            e.printStackTrace();
        }finally {
        	deinitTcp();
        }
	}

	public void wavFileTest()
	{
		try {
			System.err.println("Loading Recognizer...\n");

            ConfigurationManager cm = new ConfigurationManager(this.configURL);

            Recognizer recognizer = (Recognizer) cm.lookup("recognizer");

            /* allocate the resource necessary for the recognizer */
            recognizer.allocate();

            System.err.println("Decoding " + audioFileURL.getFile());
            System.err.println(AudioSystem.getAudioFileFormat(audioFileURL));

            StreamDataSource reader = (StreamDataSource) cm.lookup("streamDataSource");
            AudioInputStream ais 
                = AudioSystem.getAudioInputStream(audioFileURL);
            /* set the stream data source to read from the audio file */
            reader.setInputStream(ais, audioFileURL.getFile());

            /* decode the audio file */
            boolean done = false;
    	    while (!done) {
	    		/*
	    		 * This while loop will terminate after the last utterance
	    		 * in the audio file has been decoded, in which case the
	    		 * recognizer will return null.
	    		 */ 
	    		Result result = recognizer.recognize();
	    		if (result != null) {
	    		    String resultText = result.getBestResultNoFiller();
	    		    System.out.println(resultText);
	    		} else {
	    		    done = true;
	    		}
    	    }
        } catch (IOException e) {
            System.err.println("Problem when loading WavFile: " + e);
            e.printStackTrace();
        } catch (PropertyException e) {
            System.err.println("Problem configuring WavFile: " + e);
            e.printStackTrace();
        } catch (InstantiationException e) {
            System.err.println("Problem creating WavFile: " + e);
            e.printStackTrace();
        } catch (UnsupportedAudioFileException e) {
            System.err.println("Audio file format not supported: " + e);
            e.printStackTrace();
        }
	}
}


