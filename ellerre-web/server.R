library(shiny)
library(shinyjs)

# Define server logic to read selected file ----
server <- function(input, output) {

  # Enable buttons if image tab is selected
  observe({
    if(input$tabs == "LR0" | input$tabs == "LR1" | input$tabs == "LALR1" ){
      if(!is.null(input$file1)){
        shinyjs::enable("downloadImage")
        shinyjs::enable("downloadDot")
      }      
    }else{
      shinyjs::disable("downloadImage")
      shinyjs::disable("downloadDot")
    }
  })

  # Download image button
  output$downloadImage <- downloadHandler(
    filename = function() {
        paste(input$tabs,"_output.png",sep="")
    },

    content = function(file){
      filename <- paste(input$tabs, "_output.png", sep="")
      file.copy(filename, file)
    },
    contentType = "img/png"
  )

  # Download dot file button
  output$downloadDot <- downloadHandler(
    filename = function() {
        paste(input$tabs,".dot", sep="")
    },
    content = function(file){
      filename <- paste(input$tabs,".dot",sep="")
      file.copy(filename, file)
    },
    contentType = "text/dot"
  )

  # Source code for the latex reference
  output$code <- renderUI({
    tags$code(HTML(paste("@software{ellerre,", 
      "title = {EllErre: an LR automata generator},", 
      "author = {Schnorr, L. M. and Miletto, M. C. and Solórzano A. L. V.},", 
      "year = 2020,",
      "url = {https://github.com/schnorr/ellerre}",
      "}", sep = '<br/>')))
  })

  # Grammar tab
  output$grammar <- renderTable({    
    # Check input file
    req(input$file1)
    tryCatch({
        df <- read.csv(input$file1$datapath, header = FALSE)
    },
      error = function(e) {
        stop(safeError(e))
      }
    )

    return(df)
  
  }, include.colnames=FALSE)

  # First/Follow tab
  output$firstfollow <- renderTable({    

    # Check input file
    req(input$file1)
    tryCatch({
        df <- read.csv(input$file1$datapath, header = FALSE)
    },
      error = function(e) {
        stop(safeError(e))
      }
    )

    # Run ellerre
    command <- paste("./ellerre/build/firstfollow", " < ", input$file1$datapath, sep = "")
    t1 <- try(system(command, intern = TRUE))
    return(t1)
  
  }, include.colnames=FALSE)

  # LR0 tab
  output$lr0 <- renderTable({    

    # Check input file
    req(input$file1)
    tryCatch({
        df <- read.csv(input$file1$datapath, header = FALSE)
    },
      error = function(e) {
        stop(safeError(e))
      }
    )

    # Run ellerre
    command <- paste("./ellerre/build/lr0", " < ", input$file1$datapath, sep = "")
    t1 <- try(system(command, intern = TRUE))
    return(t1)
  
  }, include.colnames=FALSE)

  # LR1 tab
  output$lr1 <- renderTable({    

    # Check input file
    req(input$file1)
    tryCatch({
        df <- read.csv(input$file1$datapath, header = FALSE)
    },
      error = function(e) {
        stop(safeError(e))
      }
    )

    # Run ellerre
    command <- paste("./ellerre/build/lr1", " < ", input$file1$datapath, sep = "")
    t1 <- try(system(command, intern = TRUE))
    return(t1)
  
  }, include.colnames=FALSE)

  # LALR1 tab
  output$lalr <- renderTable({     

    # Check input file
    req(input$file1)
    tryCatch({
        df <- read.csv(input$file1$datapath, header = FALSE)
    },
      error = function(e) {
        stop(safeError(e))
      }
    )

    # Run ellerre
    command <- paste("./ellerre/build/lalr1", " < ", input$file1$datapath, sep = "")
    t1 <- try(system(command, intern = TRUE))
    return(t1)
  
  }, include.colnames=FALSE)

  # LR0 Image tab
  output$imagelr0 <- renderImage({

    inFile <- input$file1
    if (is.null(inFile)) {
      filename <- normalizePath(file.path("default.png"))
      list(src = filename, width=0.1)
    }else{
      df <- read.csv(input$file1$datapath, header = FALSE)
      command <- paste("./ellerre/build/lr0 < ", input$file1$datapath, sep = "")
      try(system(command, intern = TRUE))
      command <- paste("dot -Tpng LR0.dot -o LR0_output.png", sep="")
      try(system(paste("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu ; dot -c; ", command), intern=TRUE))

      filename <- normalizePath(file.path("LR0_output.png"))
      list(src = filename, width=800)
    }
  }, deleteFile = FALSE)

  # LR1 Image tab
  output$imagelr1 <- renderImage({

    inFile <- input$file1
    if (is.null(inFile)) {
      filename <- normalizePath(file.path("default.png"))
      list(src = filename, width=0.1)
    }else{
      df <- read.csv(input$file1$datapath, header = FALSE)
      command <- paste("./ellerre/build/lr1 < ", input$file1$datapath, sep = "")
      try(system(command, intern = TRUE))
      command <- paste("dot -Tpng LR1.dot -o LR1_output.png", sep="")
      try(system(paste("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu ; dot -c; ", command), intern=TRUE))

      filename <- normalizePath(file.path("LR1_output.png"))
      list(src = filename, width=800)
    }
  }, deleteFile = FALSE)

  # LALR1 Image tab
  output$imagelalr1 <- renderImage({

    inFile <- input$file1
    if (is.null(inFile)) {
      filename <- normalizePath(file.path("default.png"))
      list(src = filename, width=0.1)
    }else{
      df <- read.csv(input$file1$datapath, header = FALSE)
      command <- paste("./ellerre/build/lalr1 < ", input$file1$datapath, sep = "")
      try(system(command, intern = TRUE))
      command <- paste("dot -Tpng LALR1.dot -o LALR1_output.png", sep="")
      try(system(paste("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu ; dot -c; ", command), intern=TRUE))

      filename <- normalizePath(file.path("LALR1_output.png"))
      list(src = filename, width=800)
    }
  }, deleteFile = FALSE)

  # Keep Heroku server alive running the Shiny app
  output$keepAlive <- renderText({
  })

}
