library(shiny)
library(shinyjs)
library(shinythemes)

# Define UI for the EllErre app --- 
ui <- fluidPage(title="EllErre",
  tags$head(includeHTML(("google-analytics.html"))),
  tags$head(
        HTML(
          "
          <script>
          var socket_timeout_interval
          var n = 0
          $(document).on('shiny:connected', function(event) {
          socket_timeout_interval = setInterval(function(){
          Shiny.onInputChange('count', n++)
          }, 10000)
          });
          $(document).on('shiny:disconnected', function(event) {
          clearInterval(socket_timeout_interval)
          });
          </script>
          "
        )
  ),
  textOutput("keepAlive"),

  useShinyjs(),
  theme = shinytheme("united"), 

  # App title --- 
  h1(id="big-heading", "EllErre", align = "center"),
  tags$style(HTML("#big-heading{color: orange;}")),
  h4("An LR automata generator.", align = "center"),

  mainPanel(width = 11,
    div(style="display:vertical-align:bottom;horizontal-align:center;margin-left:10px",
      fluidRow(
        tags$head(tags$style(".butt{background-color:orange;} .butt{color: white;} .butt{margin-top:23px;}")),
        fluidRow(
          column(4,style=list("padding-right: 2px;"),
              fileInput("file1", "Choose File", multiple = FALSE, 
                    accept = c("text/ee", "text/comma-separated-values,text/plain", ".ee"))
          ),
          column(3,style=list("padding-left: 1px;"),
              textInput("grammartxt", "Grammar:", value ="S => A A;")
          ),
          column(3, actionButton("grammarButton", "Use my grammar", class="butt")
          ),
          column(1, downloadButton("downloadImage", "Image", class="butt")
          ),
          column(1, downloadButton("downloadDot", "DOT File", class="butt")
          )
        ),
      )
    ),
    # Tabs definition ----
    tabsetPanel(id="tabs",type = "tabs",
      tabPanel("Grammar", tableOutput("grammar"), value="grammar"),  
      tabPanel("First/Follow", tableOutput("firstfollow"), value="firstfollow"),
      tabPanel("LR(0)", tableOutput("lr0"), value="lr0"),
      tabPanel("Image LR(0)", plotOutput("imagelr0"), value="LR0"),
      tabPanel("LR(1)", tableOutput("lr1"), value="lr1"),
      tabPanel("Image LR(1)", plotOutput("imagelr1"), value="LR1"),
      tabPanel("LALR(1)", tableOutput("lalr1"), value="lalr1"),
      tabPanel("Image LALR(1)", plotOutput("imagelalr1"), value="LALR1"),
      tabPanel("About", textOutput("about"),
        fluidRow(
          column(width = 12,
            br(),
            p("EllErre is an LR automata generator created for pedagogical purposes to be used by students 
            of the Compilers course. Students can use EllErre to evaluate 
            discipline practices in an autonomous and automatic way instead of manual verification 
            of their solutions. Currently, the tool supports the generation of the first and follow set and 
            LR automata: LR(0), LR(1), LALR(1)",style="text-align:justify;color:black;background-color:lavender;padding:15px;border-radius:10px"),
          ),
          column(width = 12,
            div(
            p("Grammar specification for EllErre:"),
              tags$p(
              tags$ul(
                tags$li("Head and body separation represented by =>"),
                tags$li("Completion of each production with ;"),
                tags$li("Alternative with |"),
                tags$li("Uppercase for non-terminals, lower case for terminals"),
                tags$li("Separation between symbols with the space character"),
                tags$li("The ε (empty) symbol must be represented by the absence of symbols"),
                tags$li("The first non-terminal in the grammar is considered the head of the first production")
              )
            ),
            style="text-align:justify;color:black;background-color:lavender;padding:25px;border-radius:10px"
            ),
          ),
          column(width = 12,
            br(),
            div(
            p("If you use the EllErre information, please cite as:"),
            htmlOutput("code", placeholder = FALSE),
            br(),
            em("[Software] L. M. Schnorr, M. C. Miletto, and A. L. V. Solórzano, EllErre: an LR automata generator, 2020. URL: https://github.com/schnorr/ellerre"),
            style="text-align:justify;color:black;background-color:lavender;padding:25px;border-radius:10px"
            ),
          ),
          column(width=12,
              br(),
              p("For more information, please check the repository:",
              a(href="https://github.com/schnorr/ellerre", 
                "GitHub",target="_blank"),style="text-align:center;color:black;background-color:lavender;padding:15px;border-radius:10px")),
          br(),
          hr(),
          p(em("Developed by"),br("Lucas M. Schnorr, Marcelo C. Miletto and Ana Luisa V. Solórzano"),style="text-align:center; font-family: times"),
          p(em("Last update 13/02/2021"), style="text-align:center; font-family: times"),
          hr()
        )
      )
   )
  )
)
