library(shiny)
library(shinyjs)

# Define server logic to read selected file ----
server <- function(input, output) {

  rv <- reactiveValues() 
  grammar_path <- paste0(tempdir(), "/input.csv")

  # Enable buttons if image tab is selected
  observe({
    if(grepl("LR", input$tabs)){
      shinyjs::enable("downloadImage")
      shinyjs::enable("downloadDot")      
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

  # Observe when user input a file
  observeEvent(input$file1, {
    rv$file1=read.csv(input$file1$datapath, header = FALSE)
    system(paste("mv ", input$file1$datapath, " ", grammar_path))
  })

  # Create an event reactive for the submit data button
  observeEvent(input$grammarButton, {
    rv$file1=input$grammartxt
    write(input$grammartxt, grammar_path)
    reset('file1')
  })
  
  # Show the input grammar
  output$grammar <- renderTable({

    #If its the first access use default value
    if(is.null(rv$file1)){
      rv$file1=input$grammartxt  
      write(input$grammartxt, grammar_path)
    }
      
    return(rv$file1)

  }, include.colnames=FALSE)

  # Update tab content according to the selection
  df <- eventReactive(input$tabs, {

    if(input$tabs == "grammar" || input$tabs == "About")
      return(NULL)
      
    command <- paste("./ellerre/build/", tolower(input$tabs), " < ", grammar_path, sep = "")
    t1 <- try(system(command, intern = TRUE))
    return(t1)
  })

  # Tabs selected
  output$firstfollow <- renderTable({
    return(df())    
  }, include.colnames=FALSE)

  output$lr0 <- renderTable({
    return(df())    
  }, include.colnames=FALSE)

  output$lr1 <- renderTable({
    return(df())    
  }, include.colnames=FALSE)

  output$lalr1 <- renderTable({
    return(df())    
  }, include.colnames=FALSE)

  # Update image tabs content according to the selection
  df.image <- eventReactive(input$tabs, {

    if(input$tabs == "grammar" || input$tabs == "firstfollow" || input$tabs == "About"){
      filename <- normalizePath(file.path("default.png"))
      list(src = filename, width=0.1)
    }
    
    else{
      command <- paste0("./ellerre/build/", tolower(input$tabs), " < ", grammar_path)
      try(system(command, intern = TRUE))

      f <- paste0(input$tabs,"_output.png")
      command <- paste0("dot -Tpng ", toupper(input$tabs), ".dot -o ", f)
      try(system(paste("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu ; dot -c; ", command), intern=TRUE))
      filename <- normalizePath(file.path(f))
      list(src = f, width=800)
    }
  })

  # Image tabs selected
  output$imagelr0 <- renderImage({
    return(df.image())    
  }, deleteFile = FALSE)

  output$imagelr1 <- renderImage({
    return(df.image()) 
  }, deleteFile = FALSE)

  output$imagelalr1 <- renderImage({
    return(df.image()) 
  }, deleteFile = FALSE)

  # LALR1 Image tab
  # output$imagelalr1 <- renderImage({

  #   inFile <- input$file1
  #   if (is.null(inFile)) {
  #     filename <- normalizePath(file.path("default.png"))
  #     list(src = filename, width=0.1)
  #   }else{
  #     df <- read.csv(input$file1$datapath, header = FALSE)
  #     command <- paste("./ellerre/build/lalr1 < ", input$file1$datapath, sep = "")
  #     try(system(command, intern = TRUE))
  #     command <- paste("dot -Tpng LALR1.dot -o LALR1_output.png", sep="")
  #     try(system(paste("export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu ; dot -c; ", command), intern=TRUE))

  #     filename <- normalizePath(file.path("LALR1_output.png"))
  #     list(src = filename, width=800)
  #   }
  # }, deleteFile = FALSE)

  # Keep Heroku server alive running the Shiny app
  output$keepAlive <- renderText({
  })

}
